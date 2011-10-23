//==============================================================================
/// \file AutoUnits/DerivationParser.cpp
/// 
/// Source file for the derivation parser.
///
//  Copyright 2011 AgLeader Technology, Inc.
//==============================================================================

#include <cassert>
#include <limits>
#include <memory>

#include <QSet>

#include "DerivationParser.h"
#include "Util/Error.h"
#include "Util/ExprParser.h"

namespace AutoUnits
{

namespace 
{

using namespace Util;
class Value;
class DState;

QQueue<Token<DState>*> ParseTokens( const QString& str );

//==============================================================================
/// The state for the derivation parser.
///
class DState : public ParserState<DState>
{
public:
    //==========================================================================
    /// Constructor.
    /// 
    /// \param [in] str The string being parsed.
    /// 
    DState( const QString& str ) 
    {
        tokens = ParseTokens( str );
    }

    //==========================================================================
    /// Destructor.
    /// 
    virtual ~DState()
    {
        qDeleteAll( argstack );
    }

    /// The output stack.
    QStack<Value*> argstack;
};

//==============================================================================
/// Represents a value in the input.
/// 
class Value : public Token<DState>
{
public:
    //==========================================================================
    /// Constructor.
    ///
    /// \param [in] id The dimension id for the token.
    ///
    Value( const DimensionId& id ) : m_id( id ), m_scalar( -1 ) { }

    //==========================================================================
    /// Constructor.
    ///
    /// \param [in] scalar The scalar value for the token.
    ///
    Value( int scalar ) : m_id( DimensionId() ), m_scalar( scalar ) { }

    //==========================================================================
    /// Test whether the value is a scalar.
    /// 
    /// \return True if the value is a scalar.
    /// 
    bool IsInteger() const { return m_scalar >= 0; }
    
    //==========================================================================
    /// Get the dimension ID of the token.
    /// 
    /// \return The dimension id.
    /// 
    DimensionId GetId() const
    {
        return m_id;
    }
    
    //==========================================================================
    /// Get the integral value of the (scalar) token.
    /// 
    /// \return The value.
    /// 
    int GetScalar() const
    {
        assert( IsInteger() );
        return m_scalar;
    }

    //==========================================================================
    /// Process the token.
    /// 
    /// \param [in] state The parser state.
    /// 
    virtual void Process( DState& state )
    {
        state.argstack.push( this );
    }

private:
    /// The token's dimension id.
    DimensionId m_id;

    /// The token's scalar value.
    int m_scalar;
};

//==============================================================================
/// Represents the multiplication operator.
/// 
class Multiplication : public BinaryOperator<DState>
{
public:
    //==========================================================================
    /// Get the precedence of the operation.
    /// 
    /// \return The precedence value.
    /// 
    virtual int Precedence() const { return 1; }

    //==========================================================================
    /// Apply the operator.
    /// 
    /// \param [in] state The current parser state.
    /// 
    virtual void Apply( DState& state )
    {
        assert( state.argstack.count() >= 1 );

        std::auto_ptr<Value> rhs_p( state.argstack.pop() );

        if ( state.argstack.isEmpty() )
        {
            throw Error( "Missing operand to '*' operator." );
        }

        std::auto_ptr<Value> lhs_p( state.argstack.pop() );

        DimensionId lhs = lhs_p->GetId();
        DimensionId rhs = rhs_p->GetId();

        if ( ( lhs_p->IsInteger() && ( lhs_p->GetScalar() == 0 ) ) ||
            ( rhs_p->IsInteger() && ( rhs_p->GetScalar() == 0 ) ) )
        {
            state.argstack.push( new Value( 0 ) );
            return;
        }

        state.argstack.push( new Value( lhs * rhs ) );
    }
};

//==============================================================================
/// Represents the division operator.
/// 
class Division : public BinaryOperator<DState>
{
public:
    //==========================================================================
    /// Get the precedence of the operator.
    /// 
    /// \return The precedence.
    ///
    virtual int Precedence() const { return 1; }

    //==========================================================================
    /// Apply the operator.
    /// 
    /// \param [in] state The parser state.
    /// 
    virtual void Apply( DState& state )
    {
        assert( state.argstack.count() >= 1 );

        std::auto_ptr<Value> rhs_p( state.argstack.pop() );

        if ( state.argstack.isEmpty() )
        {
            throw Error( "Missing operand to '/' operator." );
        }

        std::auto_ptr<Value> lhs_p( state.argstack.pop() );

        DimensionId lhs = lhs_p->GetId();
        DimensionId rhs = rhs_p->GetId();

        if ( lhs_p->IsInteger() && ( lhs_p->GetScalar() == 0 ) )
        {
            state.argstack.push( new Value( 0 ) );
            return;
        }

        if ( rhs_p->IsInteger() && ( rhs_p->GetScalar() == 0 ) )
        {
            throw Error( "Division by zero." );
        }

        state.argstack.push( new Value( lhs / rhs ) );
    }
};

//==============================================================================
/// Represents the exponentiation operator.
/// 
class Exponentiation : public BinaryOperator<DState>
{
public:
    //==========================================================================
    /// Get the precedence of the operator.
    /// 
    /// \return The precedence.
    /// 
    virtual int Precedence() const { return 2; }

    //==========================================================================
    /// Apply the operator.
    /// 
    /// \param [in] state The parser state.
    /// 
    virtual void Apply( DState& state )
    {
        assert( state.argstack.count() >= 1 );

        std::auto_ptr<Value> rhs_p( state.argstack.pop() );

        if ( state.argstack.isEmpty() )
        {
            throw Error( "Missing operand to '^' operator." );
        }

        std::auto_ptr<Value> lhs_p( state.argstack.pop() );

        DimensionId lhs = lhs_p->GetId();

        if ( lhs_p->IsInteger() )
        {
            throw Error(
                "Left-hand side of '^' operator was not a dimension." );
        }

        if ( !rhs_p->IsInteger() )
        {
            throw Error( 
                "Right-hand side of '^' operator was not an integer." );
        }

        state.argstack.push( new Value( lhs ^ rhs_p->GetScalar() ) );
    }
};

//==============================================================================
/// Tokenize the given derivation string.
/// 
/// \param [in] str The string to tokenize.
/// 
/// \return The tokens.
/// 
QQueue<Token<DState>*> ParseTokens( const QString& str )
{
    QQueue<Token<DState>*> result;
    int i = 0;
    while ( i < str.count() )
    {
        if ( str[i] == '^' )
        {
            result.enqueue( new Exponentiation );
            i++;
        }
        else if ( str[i] == '*' )
        {
            result.enqueue( new Multiplication );
            i++;
        }
        else if ( str[i] == '/' )
        {
            result.enqueue( new Division );
            i++;
        }
        else if ( str[i] == '(' )
        {
            result.enqueue( new LParen<DState> );
            i++;
        }
        else if ( str[i] == ')' )
        {
            result.enqueue( new RParen<DState> );
            i++;
        }
        else if ( str[i].isDigit() )
        {
            QString num_text = "";
            while ( i < str.count() && str[i].isDigit() )
            {
                num_text += str[i++];
            }

            result.enqueue( new Value( num_text.toInt() ) );
        }
        else if ( str[i].isLetter() )
        {
            QString id_text = "";
            while ( i < str.count() && str[i].isLetterOrNumber() )
            {
                id_text += str[i++];
            }

            DimensionId id;
            id[id_text] = 1;
            result.enqueue( new Value( id ) );
        }
        else if ( str[i].isSpace() )
        {
            i++;
        }
        else
        {
            throw Error( "Unmatched input: " + str.mid( i ) );
        }
    }

    return result;
}

} // namespace

//==============================================================================
/// Parse a derived dimension derivation string.
/// 
/// \param [in] str The derivation.
/// 
/// \return The dimension ID.
/// 
DimensionId ParseDerivation( const QString& str )
{
    DState state( str );
    ParseExpr( state );

    if ( state.argstack.count() != 1 )
    {
        throw Error( "Syntax error (real helpful, huh?)" );
    }

    return state.argstack.top()->GetId();
}

} // namespace AutoUnits

