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

#include <QQueue>
#include <QSet>
#include <QStack>

#include "DerivationParser.h"

namespace AutoUnits
{

namespace 
{

class Token;
class Value;
class Operator;

struct ParserState
{
public:
    ~ParserState()
    {
        qDeleteAll( tokens );
        qDeleteAll( argstack );
        qDeleteAll( opstack );
    }
    QQueue<Token*> tokens;
    QStack<Value*> argstack;
    QStack<Operator*> opstack;
};

class Token
{
public:
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
        state.argstack.push( this );
    }

    DimensionId m_id;
    int m_scalar;
};
class Operator : public Token
{
public:
    /// Apply the operator to the values on the stack.
    //
    /// param [in] ids The values.
    virtual void Apply( QStack<Value*>& ids ) = 0;
    /// Get the precedence of the operator.
    /// \return The precedence.
    virtual int Precedence() const = 0;
    virtual bool IsLParen() const { return false; }
};

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
                other_p->Apply( state.argstack );
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

class LParen : public Operator
{
    virtual int Precedence() const { return std::numeric_limits<int>::min(); }
    virtual bool IsLParen() const { return true; }
    virtual void Process( ParserState& state )
    {
        state.opstack.push( this );
    }

    virtual void Apply( QStack<Value*>& ) { }
};

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
            op_p->Apply( state.argstack );

            saw_lparen = op_p->IsLParen();
        }

        if ( !saw_lparen )
        {
            throw DerivationError( "Unexpected ')'." );
        }
    }
    
    virtual void Apply( QStack<Value*>& ) { assert( false ); }
};

class Multiplication : public BinaryOperator
{
public:
    virtual int Precedence() const { return 1; }
    virtual void Apply( QStack<Value*>& stack )
    {
        assert( stack.count() >= 1 );

        std::auto_ptr<Value> rhs_p( stack.pop() );

        if ( stack.isEmpty() )
        {
            throw DerivationError( "Missing operand to '*' operator." );
        }

        std::auto_ptr<Value> lhs_p( stack.pop() );

        DimensionId lhs = lhs_p->GetId();
        DimensionId rhs = rhs_p->GetId();

        if ( ( lhs_p->IsInteger() && ( lhs_p->GetScalar() == 0 ) ) ||
            ( rhs_p->IsInteger() && ( rhs_p->GetScalar() == 0 ) ) )
        {
            stack.push( new Value( 0 ) );
            return;
        }

        stack.push( new Value( lhs * rhs ) );
    }
};

class Division : public BinaryOperator
{
public:
    virtual int Precedence() const { return 1; }
    virtual void Apply( QStack<Value*>& stack )
    {
        assert( stack.count() >= 1 );

        std::auto_ptr<Value> rhs_p( stack.pop() );

        if ( stack.isEmpty() )
        {
            throw DerivationError( "Missing operand to '/' operator." );
        }

        std::auto_ptr<Value> lhs_p( stack.pop() );

        DimensionId lhs = lhs_p->GetId();
        DimensionId rhs = rhs_p->GetId();

        if ( lhs_p->IsInteger() && ( lhs_p->GetScalar() == 0 ) )
        {
            stack.push( new Value( 0 ) );
            return;
        }

        if ( rhs_p->IsInteger() && ( rhs_p->GetScalar() == 0 ) )
        {
            throw DerivationError( "Division by zero." );
        }

        stack.push( new Value( lhs / rhs ) );
    }
};

class Exponentiation : public BinaryOperator
{
public:
    virtual int Precedence() const { return 2; }
    virtual void Apply( QStack<Value*>& stack )
    {
        assert( stack.count() >= 1 );

        std::auto_ptr<Value> rhs_p( stack.pop() );

        if ( stack.isEmpty() )
        {
            throw DerivationError( "Missing operand to '^' operator." );
        }

        std::auto_ptr<Value> lhs_p( stack.pop() );

        DimensionId lhs = lhs_p->GetId();

        if ( lhs_p->IsInteger() )
        {
            throw DerivationError(
                "Left-hand side of '^' operator was not a dimension." );
        }

        if ( !rhs_p->IsInteger() )
        {
            throw DerivationError( 
                "Right-hand side of '^' operator was not an integer." );
        }

        stack.push( new Value( lhs ^ rhs_p->GetScalar() ) );
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
/// Constructor.
/// 
/// \param [in] description The description of the error.
/// 
DerivationError::DerivationError( const QString& description ) : 
    m_desc( description ) 
{ }

//==============================================================================
/// Converts a derivation error into a string.
/// 
/// \return The string representation of the error.
/// 
DerivationError::operator QString() const
{
    return m_desc;
}

//==============================================================================
/// Parse a derived dimension derivation string.
/// 
/// \param [in] str The derivation.
/// 
/// \return The dimension ID.
/// 
DimensionId ParseDerivation( const QString& str )
{
    ParserState state;

    state.tokens = ParseTokens( str );

    while ( !state.tokens.isEmpty() )
    {
        Token *tok_p( state.tokens.dequeue() );
        tok_p->Process( state );
    }

    while ( !state.opstack.isEmpty() )
    {
        std::auto_ptr<Operator> op_p( state.opstack.pop() );

        if ( op_p->IsLParen() )
        {
            throw DerivationError( "Unmatched '('." );
        }
        op_p->Apply( state.argstack );
    }

    if ( state.argstack.count() != 1 )
    {
        throw DerivationError( "Syntax error (real helpful, huh?)" );
    }

    return state.argstack.top()->GetId();
}

} // namespace AutoUnits

