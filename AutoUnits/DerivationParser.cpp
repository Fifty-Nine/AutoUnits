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
    virtual bool IsLParen() const { return false; }
};

class BinaryOperator : public Operator
{
public:
    /// Get the precedence of the operator.
    /// \return The precedence.
    virtual int Precedence() const = 0;

    /// Process the operator.
    /// \param [in] state The current parser state.
    virtual void Process( ParserState& state )
    {
        while ( !state.opstack.isEmpty() )
        {
            BinaryOperator *other_p = 
                static_cast<BinaryOperator*>( state.opstack.top() );
            if ( Precedence() <= other_p->Precedence() )
            {
                state.opstack.pop();
                other_p->Apply( state.argstack );
                delete other_p;
            }
            else
            {
                break;
            }
        }

        state.opstack.push( this );
    }
};

class LParen : public Operator
{
    virtual bool IsLParen() const { return true; }
    virtual int Precedence() const { return std::numeric_limits<int>::max(); }
    virtual void Process( ParserState& state )
    {
        state.opstack.push( this );
    }

    virtual void Apply( QStack<Value*>& ) { }
};

class RParen : public Operator
{
    virtual void Process( ParserState& state )
    {
        assert( !state.opstack.isEmpty() );
        
        bool saw_lparen = false;
        while ( !saw_lparen && !state.opstack.isEmpty() )
        {
            Operator *op_p = state.opstack.pop();
            op_p->Apply( state.argstack );

            saw_lparen = op_p->IsLParen();
            delete op_p;
        }

        delete this;
    }
    
    virtual void Apply( QStack<Value*>& ) { assert( false ); }
};

class Multiplication : public BinaryOperator
{
public:
    virtual int Precedence() const { return 1; }
    virtual void Apply( QStack<Value*>& stack )
    {
        Value *rhs_p = stack.pop();
        Value *lhs_p = stack.pop();

        DimensionId lhs = lhs_p->GetId();
        DimensionId rhs = rhs_p->GetId();

        delete lhs_p;
        delete rhs_p;

        DimensionId result;

        QSet<QString> keys( lhs.keys().toSet() );
        keys.unite( rhs.keys().toSet() );

        for ( QSet<QString>::const_iterator it = keys.begin(); 
            it != keys.end(); ++it )
        {
            result[*it] = ( lhs[*it] + rhs[*it] );
        }

        stack.push( new Value( result ) );
    }
};

class Division : public BinaryOperator
{
public:
    virtual int Precedence() const { return 1; }
    virtual void Apply( QStack<Value*>& stack )
    {
        Value *rhs_p = stack.pop();
        Value *lhs_p = stack.pop();

        DimensionId lhs = lhs_p->GetId();
        DimensionId rhs = rhs_p->GetId();

        delete lhs_p;
        delete rhs_p;

        DimensionId result;
        QSet<QString> keys( lhs.keys().toSet() );
        keys.unite( rhs.keys().toSet() );

        for ( QSet<QString>::const_iterator it = keys.begin(); 
            it != keys.end(); ++it )
        {
            result[*it] = ( lhs[*it] - rhs[*it] );
        }

        stack.push( new Value( result ) );
    }
};

class Exponentiation : public BinaryOperator
{
public:
    virtual int Precedence() const { return 2; }
    virtual void Apply( QStack<Value*>& stack )
    {
        Value *rhs_p = stack.pop();
        Value *lhs_p = stack.pop();

        DimensionId lhs = lhs_p->GetId();

        assert( rhs_p->IsInteger() );
        int rhs = rhs_p->GetScalar();

        delete lhs_p;
        delete rhs_p;

        DimensionId result;

        QSet<QString> keys = lhs.keys().toSet();
        for ( DimensionId::const_iterator it = lhs.begin(); 
            it != lhs.end(); ++it )
        {
            result[it.key()] = ( it.value() * rhs );
        }

        stack.push( new Value( result ) );
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
    ParserState state;

    state.tokens = ParseTokens( str );

    while ( !state.tokens.isEmpty() )
    {
        std::auto_ptr<Token> tok_p( state.tokens.dequeue() );
        tok_p->Process( state );
        tok_p.release();
    }

    while ( !state.opstack.isEmpty() )
    {
        std::auto_ptr<Operator> op_p( state.opstack.pop() );
        op_p->Apply( state.argstack );
    }

    assert( state.argstack.count() == 1 );

    return state.argstack.top()->GetId();
}

} // namespace AutoUnits

