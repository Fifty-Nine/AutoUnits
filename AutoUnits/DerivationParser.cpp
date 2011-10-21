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

QQueue<Token*> ParseTokens( const QString& str );

//==============================================================================
/// The state for the derivation parser.
///
class DerivationParserState : public Util::ParserState
{
public:
    //==========================================================================
    /// Constructor.
    /// 
    /// \param [in] str The string being parsed.
    /// 
    DerivationParserState( const QString& str ) 
    {
        tokens = ParseTokens( str );
    }

    //==========================================================================
    /// Destructor.
    /// 
    virtual ~DerivationParserState()
    {
        qDeleteAll( argstack );
    }

    /// The output stack.
    QStack<Value*> argstack;
};

//==============================================================================
/// Represents a value in the input.
/// 
class Value : public Token
{
public:
    Value( const DimensionId& id ) : m_id( id ), m_scalar( -1 ) { }
    Value( int scalar ) : m_id( DimensionId() ), m_scalar( scalar ) { }

    virtual bool IsInteger() const { return m_scalar >= 0; }
    DimensionId GetId() const
    {
        return m_id;
    }

    int GetScalar() const
    {
        assert( IsInteger() );
        return m_scalar;
    }

    virtual void Process( ParserState& state )
    {
        DerivationParserState& derived( 
            static_cast<DerivationParserState&>( state ) );
        derived.argstack.push( this );
    }

    DimensionId m_id;
    int m_scalar;
};

//==============================================================================
/// Represents the multiplication operator.
/// 
class Multiplication : public BinaryOperator
{
public:
    virtual int Precedence() const { return 1; }
    virtual void Apply( ParserState& state )
    {
        DerivationParserState& derived( 
            static_cast<DerivationParserState&>( state ) );

        assert( derived.argstack.count() >= 1 );

        std::auto_ptr<Value> rhs_p( derived.argstack.pop() );

        if ( derived.argstack.isEmpty() )
        {
            throw Error( "Missing operand to '*' operator." );
        }

        std::auto_ptr<Value> lhs_p( derived.argstack.pop() );

        DimensionId lhs = lhs_p->GetId();
        DimensionId rhs = rhs_p->GetId();

        if ( ( lhs_p->IsInteger() && ( lhs_p->GetScalar() == 0 ) ) ||
            ( rhs_p->IsInteger() && ( rhs_p->GetScalar() == 0 ) ) )
        {
            derived.argstack.push( new Value( 0 ) );
            return;
        }

        derived.argstack.push( new Value( lhs * rhs ) );
    }
};

//==============================================================================
/// Represents the division operator.
/// 
class Division : public BinaryOperator
{
public:
    virtual int Precedence() const { return 1; }
    virtual void Apply( ParserState& state )
    {
        DerivationParserState& derived( 
            static_cast<DerivationParserState&>( state ) );
        assert( derived.argstack.count() >= 1 );

        std::auto_ptr<Value> rhs_p( derived.argstack.pop() );

        if ( derived.argstack.isEmpty() )
        {
            throw Error( "Missing operand to '/' operator." );
        }

        std::auto_ptr<Value> lhs_p( derived.argstack.pop() );

        DimensionId lhs = lhs_p->GetId();
        DimensionId rhs = rhs_p->GetId();

        if ( lhs_p->IsInteger() && ( lhs_p->GetScalar() == 0 ) )
        {
            derived.argstack.push( new Value( 0 ) );
            return;
        }

        if ( rhs_p->IsInteger() && ( rhs_p->GetScalar() == 0 ) )
        {
            throw Error( "Division by zero." );
        }

        derived.argstack.push( new Value( lhs / rhs ) );
    }
};

//==============================================================================
/// Represents the exponentiation operator.
/// 
class Exponentiation : public BinaryOperator
{
public:
    virtual int Precedence() const { return 2; }
    virtual void Apply( ParserState& state )
    {
        DerivationParserState& derived( 
            static_cast<DerivationParserState&>( state ) );
        assert( derived.argstack.count() >= 1 );

        std::auto_ptr<Value> rhs_p( derived.argstack.pop() );

        if ( derived.argstack.isEmpty() )
        {
            throw Error( "Missing operand to '^' operator." );
        }

        std::auto_ptr<Value> lhs_p( derived.argstack.pop() );

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

        derived.argstack.push( new Value( lhs ^ rhs_p->GetScalar() ) );
    }
};

//==============================================================================
/// Tokenize the given derivation string.
/// 
/// \param [in] str The string to tokenize.
/// 
/// \return The tokens.
/// 
QQueue<Token*> ParseTokens( const QString& str )
{
    QQueue<Token*> result;
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
            result.enqueue( new LParen );
            i++;
        }
        else if ( str[i] == ')' )
        {
            result.enqueue( new RParen );
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
            return QQueue<Token*>();
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
    DerivationParserState state( str );

    Util::ParseExpr( state );

    if ( state.argstack.count() != 1 )
    {
        throw Error( "Syntax error (real helpful, huh?)" );
    }

    return state.argstack.top()->GetId();
}

} // namespace AutoUnits

