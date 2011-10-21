#ifndef AUTO_UNITS_UTIL_EXPR_PARSER_H
#define AUTO_UNITS_UTIL_EXPR_PARSER_H
//==============================================================================
/// \file AutoUnits/Util/ExprParser.h
/// 
/// An object oriented expression parsing utility.
///
//  Copyright 2011 AgLeader Technology, Inc.
//==============================================================================

#include <cassert>
#include <limits>
#include <memory>

#include <QQueue>
#include <QStack>

#include "Util/Error.h"

namespace AutoUnits
{
namespace Util
{

class Token;
class Operator;

//==============================================================================
/// The base class for the state used by the parser.
/// 
class ParserState
{
public:
    virtual ~ParserState()
    {
        qDeleteAll( tokens );
        qDeleteAll( opstack );
    }

    QQueue<Token*> tokens;
    QStack<Operator*> opstack;
};

//==============================================================================
/// The base class for all tokens consumed by the parser.
/// 
class Token
{
public:
    /// Destructor.
    virtual ~Token() { }

    /// Do whatever needs to be done to process the token with the given parser
    /// state. 
    /// 
    /// \param [in] state The parser state.
    /// 
    /// \note The token takes ownership of itself for the lifetime of this
    /// function. In other words, the token either needs to be deleted or
    /// pushed onto one of the stacks before the function exits. 
    /// 
    virtual void Process( ParserState& state ) = 0;
};

//==============================================================================
/// The base class for all operators consumed by the parser.
/// 
class Operator : public Token
{
public:
    /// Apply the operator to the values on the stack.
    //
    /// param [in] ids The values.
    virtual void Apply( ParserState& ids ) = 0;
    /// Get the precedence of the operator.
    /// \return The precedence.
    virtual int Precedence() const = 0;
    virtual bool IsLParen() const { return false; }
};

//==============================================================================
/// The base class for all binary operations consumed by the parser.
/// 
class BinaryOperator : public Operator
{
public:
    /// Process the operator.
    /// \param [in] state The current parser state.
    virtual void Process( ParserState& state )
    {
        std::auto_ptr<BinaryOperator> guard( this );
        while ( !state.opstack.isEmpty() )
        {
            std::auto_ptr<Operator> other_p( state.opstack.top() );
            if ( Precedence() <= other_p->Precedence() )
            {
                state.opstack.pop();
                other_p->Apply( state );
            }
            else
            {
                other_p.release();
                break;
            }
        }

        state.opstack.push( guard.release() );
    }
};

//==============================================================================
/// A token instantiated for a left parenthesis.
/// 
class LParen : public Operator
{
    virtual int Precedence() const { return std::numeric_limits<int>::min(); }
    virtual bool IsLParen() const { return true; }
    virtual void Process( ParserState& state )
    {
        state.opstack.push( this );
    }

    virtual void Apply( ParserState& ) { }
};

//==============================================================================
/// A token instantiated for a right parenthesis.
/// 
class RParen : public Operator
{
    virtual int Precedence() const 
    { 
        // Should never be on the opstack.
        assert( false ); 
    }

    virtual void Process( ParserState& state )
    {
        std::auto_ptr<RParen> guard( this );
        
        bool saw_lparen = false;
        while ( !saw_lparen && !state.opstack.isEmpty() )
        {
            std::auto_ptr<Operator> op_p( state.opstack.pop() );
            op_p->Apply( state );

            saw_lparen = op_p->IsLParen();
        }

        if ( !saw_lparen )
        {
            throw Error( "Unexpected ')'." );
        }
    }
    
    virtual void Apply( ParserState& ) { assert( false ); }
};

void ParseExpr( ParserState& state );

} // namespace Util
} // namespace AutoUnits

#endif // AUTO_UNITS_UTIL_EXPR_PARSER_H
