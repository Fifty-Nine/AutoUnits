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

template<class ST>
class Token;
template<class ST>
class Operator;

//==============================================================================
/// The base class for the state used by the parser.
///
template<class STATE> 
class ParserState
{
public:
    //==========================================================================
    /// Destructor.
    /// 
    virtual ~ParserState()
    {
        qDeleteAll( tokens );
        qDeleteAll( opstack );
    }

    /// The token input queue.
    QQueue<Token<STATE>*> tokens;

    /// The operator stack.
    QStack<Operator<STATE>*> opstack;
};

//==============================================================================
/// The base class for all tokens consumed by the parser.
/// 
/// \tparam STATE The parser state type.
/// 
template<class STATE> 
class Token
{
public:
    /// Destructor.
    virtual ~Token() { }

    //==========================================================================
    /// Do whatever needs to be done to process the token with the given parser
    /// state. 
    /// 
    /// \param [in] state The parser state.
    /// 
    /// \note The token takes ownership of itself for the lifetime of this
    /// function. In other words, the token either needs to be deleted or
    /// pushed onto one of the stacks before the function exits. 
    /// 
    virtual void Process( STATE& state ) = 0;
};

//==============================================================================
/// The base class for all operators consumed by the parser.
/// 
template<class STATE>
class Operator : public Token<STATE>
{
public:
    //==========================================================================
    /// Apply the operator.
    ///
    /// param [in] state The parser state.
    /// 
    virtual void Apply( STATE& state ) = 0;

    //==========================================================================
    /// Get the precedence of the operator.
    /// 
    /// \return The precedence.
    /// 
    virtual int Precedence() const = 0;

    //==========================================================================
    /// Test whether the operator is a left parenthesis.
    /// 
    /// \return True if the operator is an lparen.
    /// 
    virtual bool IsLParen() const { return false; }
};

//==============================================================================
/// The base class for all binary operations consumed by the parser.
///
template<class STATE> 
class BinaryOperator : public Operator<STATE>
{
public:
    //==========================================================================
    /// Process the operator.
    /// 
    /// \param [in] state The current parser state.
    /// 
    virtual void Process( STATE& state )
    {
        std::auto_ptr<BinaryOperator> guard( this );
        while ( !state.opstack.isEmpty() )
        {
            std::auto_ptr<Operator<STATE> > other_p( 
                state.opstack.top() );
            if ( this->Precedence() <= other_p->Precedence() )
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
template<class STATE>
class LParen : public Operator<STATE>
{
    //==========================================================================
    /// Get the precedence of the operator.
    /// 
    /// \return The precedence.
    /// 
    virtual int Precedence() const { return std::numeric_limits<int>::min(); }

    //==========================================================================
    /// Test whether the operator is a left parenthesis.
    /// 
    /// \return True.
    /// 
    virtual bool IsLParen() const { return true; }

    //==========================================================================
    /// Process the operator.
    /// 
    /// \param [in] state The parser state.
    /// 
    virtual void Process( STATE& state )
    {
        state.opstack.push( this );
    }

    //==========================================================================
    /// Apply the operator.
    ///
    virtual void Apply( STATE& ) { }
};

//==============================================================================
/// A token instantiated for a right parenthesis.
/// 
template<class STATE>
class RParen : public Token<STATE>
{
    //==========================================================================
    /// Process the token.
    /// 
    /// \param [in] state The state.
    /// 
    virtual void Process( STATE& state )
    {
        std::auto_ptr<RParen> guard( this );
        
        bool saw_lparen = false;
        while ( !saw_lparen && !state.opstack.isEmpty() )
        {
            std::auto_ptr<Operator<STATE> > op_p( state.opstack.pop() );
            op_p->Apply( state );

            saw_lparen = op_p->IsLParen();
        }

        if ( !saw_lparen )
        {
            throw Error( "Unexpected ')'." );
        }
    }
};

//==============================================================================
/// Parse an expression in the given string.
/// 
template<class STATE>
void ParseExpr( STATE& state )
{
    while ( !state.tokens.isEmpty() )
    {
        Token<STATE> *tok_p( state.tokens.dequeue() );
        tok_p->Process( state );
    }

    while ( !state.opstack.isEmpty() )
    {
        std::auto_ptr<Operator<STATE> > op_p( state.opstack.pop() );

        if ( op_p->IsLParen() )
        {
            throw Error( "Unmatched '('." );
        }
        op_p->Apply( state );
    }
}

} // namespace Util
} // namespace AutoUnits

#endif // AUTO_UNITS_UTIL_EXPR_PARSER_H
