//==============================================================================
/// \file AutoUnits/DerivationParser.cpp
/// 
/// Source file for the derivation parser.
///
//  Copyright 2011 AgLeader Technology, Inc.
//==============================================================================

#include <QQueue>
#include <QSet>
#include <QStack>

#include "DerivationParser.h"

namespace AutoUnits
{

namespace 
{

class Token;
class Operator;

struct ParserState
{
public:
    QQueue<Token*> tokens;
    QStack<DimensionId> idstack;
    QStack<Operator*> opstack;
};

class Token
{
public:
    virtual void Process( ParserState& state ) = 0;
};

class Operator : public Token
{
public:
    /// Get the precedence of the operator.
    /// \return The precedence.
    virtual int Precedence() const = 0;
    /// Apply the operator to the values on the stack.
    /// param [in] ids The values.
    virtual void Apply( QStack<DimensionId>& ids ) = 0;

    enum Associativity
    {
        Left,
        Right
    };

    virtual Associativity GetAssociativity() const
    {
        return Left;
    }

    /// Process the operator.
    /// \param [in] state The current parser state.
    virtual void Process( ParserState& state )
    {
        while ( !state.opstack.isEmpty() )
        {
            Operator *other_p = state.opstack.top();
            if ( ( ( GetAssociativity() == Left ) &&
                ( Precedence() <= other_p->Precedence() ) ) ||
                ( ( GetAssociativity() == Right ) &&
                ( Precedence() < other_p->Precedence() ) ) )
            {
                state.opstack.pop();
                other_p->Apply( state.idstack );
                delete other_p;
            }
        }

        state.opstack.push( this );
    }
};

class Multiplication : public Operator
{
public:
    virtual int Precedence() const { return 1; }
    virtual void Apply( QStack<DimensionId>& stack )
    {
        DimensionId rhs = stack.pop();
        DimensionId lhs = stack.pop();
        DimensionId result;

        QSet<QString> keys( lhs.keys().toSet() );
        keys.unite( rhs.keys().toSet() );

        for ( QSet<QString>::const_iterator it = keys.begin(); 
            it != keys.end(); ++it )
        {
            result[*it] = ( lhs[*it] + rhs[*it] );
        }

        stack.push( result );
    }
};

class Division : public Operator
{
public:
    virtual int Precedence() const { return 1; }
    virtual void Apply( QStack<DimensionId>& stack )
    {
        DimensionId rhs = stack.pop();
        DimensionId lhs = stack.pop();
        DimensionId result;

        QSet<QString> keys( lhs.keys().toSet() );
        keys.unite( rhs.keys().toSet() );

        for ( QSet<QString>::const_iterator it = keys.begin(); 
            it != keys.end(); ++it )
        {
            result[*it] = ( lhs[*it] - rhs[*it] );
        }

        stack.push( result );
    }
};

class Identifier : public Token
{
public:
    Identifier( const QString& text ) : m_text( text ) { }
    DimensionId GetId() const
    {
        DimensionId result;

        if ( m_text != "1" )
        {
            result[m_text] = 1;
        }

        return result;
    }

    virtual void Process( ParserState& state )
    {
        state.idstack.push( GetId() );
        delete this;
    }

    QString m_text;
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
            /// \todo 
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
        else if ( str[i] == '1' )
        {
            result.enqueue( new Identifier( "1" ) );
            i++;
            
            if ( i < str.count() && str[i] == '.' )
            {
                i++;
                if ( i < str.count() && str[i] == '0' ) i++;
            }
        }
        else if ( str[i].isLetter() )
        {
            QString id = "";
            while ( i < str.count() && str[i].isLetterOrNumber() )
            {
                id += str[i++];
            }

            result.enqueue( new Identifier( id ) );
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
        Token *tok_p = state.tokens.dequeue();
        tok_p->Process( state );
    }

    while ( !state.opstack.isEmpty() )
    {
        state.opstack.pop()->Process( state );
    }

    qDeleteAll( state.opstack );

    return state.idstack.top();
}

} // namespace AutoUnits

