//==============================================================================
/// \file AutoUnits/ConversionParser.cpp
/// 
/// Source file for the conversion function parser.
///
//  Copyright 2011 AgLeader Technology, Inc.
//==============================================================================

#include <QRegExp>

#include "ConversionParser.h"
#include "Types/Conversion.h"
#include "Util/ExprParser.h"

namespace AutoUnits
{

using namespace Util;

namespace 
{

class State;
QQueue<Token<State>*> Tokenize( const QString& str );

//==============================================================================
/// The parser state for the conversion function parser.
/// 
class State : public ParserState<State>
{
public:
    State( const QString& str ) 
    {
        tokens = Tokenize( str );
    }

    virtual ~State()
    {
        qDeleteAll( convstack );
    }

    QStack<Conversion*> convstack;
};

//==============================================================================
/// Processes a constant token.
/// 
class ConstantToken : public Token<State>
{
public:
    //==========================================================================
    /// Constructor.
    /// 
    /// \param [in] value The token's value.
    /// 
    ConstantToken( double value ) : 
        m_value( value )
    { }

    //==========================================================================
    /// Get the constant value.
    /// 
    /// \return The value.
    ///
    double GetConstant() const
    {
        return m_value;
    }

    //==========================================================================
    /// Process the token.
    /// 
    /// \param [in] state The parser state.
    /// 
    virtual void Process( State& state )
    {
        state.convstack.push( new Conversions::Constant( m_value ) );
        delete this;
    }

private:
    /// The constant's value.
    double m_value;
};

//==============================================================================
/// Processes a value placeholder token.
/// 
class ValueToken : public Token<State>
{
public:
    //==========================================================================
    /// Process the token.
    ///
    /// \param [in] state The parser state.
    ///
    virtual void Process( State& state )
    {
        state.convstack.push( new Conversions::Value );
        delete this;
    }
};

//==============================================================================
/// Processes a '+' token.
///
class AddToken : public BinaryOperator<State>
{
public:
    //==========================================================================
    /// Get the precedence of the operator.
    /// 
    /// \return The precedence.
    /// 
    virtual int Precedence() const 
    {
        return 0;
    }

    //==========================================================================
    /// Apply the operator.
    /// 
    /// \param [in] state The current parser state.
    /// 
    virtual void Apply( State& state )
    {
        assert( state.convstack.count() >= 1 );

        std::auto_ptr<Conversion> rhs_p( state.convstack.pop() );

        if ( state.convstack.isEmpty() )
        {
            throw Error( "Missing operand to '+' operator." );
        }

        std::auto_ptr<Conversion> lhs_p( state.convstack.pop() );

        if ( lhs_p->IsConstant() && rhs_p->IsConstant() )
        {
            state.convstack.push( new Conversions::Constant( 
                lhs_p->Eval( 0.0 ) + rhs_p->Eval( 0.0 ) ) );
            return;
        }
        
        state.convstack.push( new Conversions::AddOp( lhs_p, rhs_p ) );
    }
};


//==============================================================================
/// Processes a '-' token.
///
class SubToken : public BinaryOperator<State>
{
public:
    //==========================================================================
    /// Get the precedence of the operator.
    /// 
    /// \return The precedence.
    /// 
    virtual int Precedence() const 
    {
        return 0;
    }

    //==========================================================================
    /// Apply the operator.
    /// 
    /// \param [in] state The current parser state.
    /// 
    virtual void Apply( State& state )
    {
        assert( state.convstack.count() >= 1 );

        std::auto_ptr<Conversion> rhs_p( state.convstack.pop() );

        if ( state.convstack.isEmpty() )
        {
            throw Error( "Missing operand to '-' operator." );
        }

        std::auto_ptr<Conversion> lhs_p( state.convstack.pop() );

        if ( lhs_p->IsConstant() && rhs_p->IsConstant() )
        {
            state.convstack.push( new Conversions::Constant( 
                lhs_p->Eval( 0.0 ) - rhs_p->Eval( 0.0 ) ) );
            return;
        }
        
        state.convstack.push( new Conversions::SubOp( lhs_p, rhs_p ) );
    }
};


//==============================================================================
/// Processes a '*' token.
///
class MultToken : public BinaryOperator<State>
{
public:
    //==========================================================================
    /// Get the precedence of the operator.
    /// 
    /// \return The precedence.
    /// 
    virtual int Precedence() const 
    {
        return 1;
    }

    //==========================================================================
    /// Apply the operator.
    /// 
    /// \param [in] state The current parser state.
    /// 
    virtual void Apply( State& state )
    {
        assert( state.convstack.count() >= 1 );

        std::auto_ptr<Conversion> rhs_p( state.convstack.pop() );

        if ( state.convstack.isEmpty() )
        {
            throw Error( "Missing operand to '*' operator." );
        }

        std::auto_ptr<Conversion> lhs_p( state.convstack.pop() );

        if ( lhs_p->IsConstant() && rhs_p->IsConstant() )
        {
            state.convstack.push( new Conversions::Constant( 
                lhs_p->Eval( 0.0 ) * rhs_p->Eval( 0.0 ) ) );
            return;
        }
        
        state.convstack.push( new Conversions::MultOp( lhs_p, rhs_p ) );
    }
};


//==============================================================================
/// Processes a '/' token.
///
class DivToken : public BinaryOperator<State>
{
public:
    //==========================================================================
    /// Get the precedence of the operator.
    /// 
    /// \return The precedence.
    /// 
    virtual int Precedence() const 
    {
        return 1;
    }

    //==========================================================================
    /// Apply the operator.
    /// 
    /// \param [in] state The current parser state.
    /// 
    virtual void Apply( State& state )
    {
        assert( state.convstack.count() >= 1 );

        std::auto_ptr<Conversion> rhs_p( state.convstack.pop() );

        if ( state.convstack.isEmpty() )
        {
            throw Error( "Missing operand to '/' operator." );
        }

        std::auto_ptr<Conversion> lhs_p( state.convstack.pop() );

        if ( rhs_p->IsConstant() && ( rhs_p->Eval( 0.0 ) == 0.0 ) )
        {
            // I used the wording here because probably won't detect
            // every instance due to floating point fuzziness.
            throw Error( 
                "Division by zero detected in conversion expression." );
        }

        if ( lhs_p->IsConstant() && rhs_p->IsConstant() )
        {
            state.convstack.push( new Conversions::Constant( 
                lhs_p->Eval( 0.0 ) / rhs_p->Eval( 0.0 ) ) );
            return;
        }
        
        state.convstack.push( new Conversions::DivOp( lhs_p, rhs_p ) );
    }
};

//==============================================================================
/// Tokenize the input string.
/// 
/// \param [in] str The string.
/// 
QQueue<Token<State>*> Tokenize( const QString& str )
{
    QQueue<Token<State>*> result;
    QRegExp num_re( "[0-9]+\\.?|[0-9]*\\.[0-9]+" );

    int i = 0;
    while ( i < str.count() )
    {
        QChar c = str[i];
        if ( c == '*' )
        {
            result.enqueue( new MultToken );
            i++;
        }
        else if ( c == '/' )
        {
            result.enqueue( new DivToken );
            i++;
        }
        else if ( c == '+' )
        {
            result.enqueue( new AddToken );
            i++;
        }
        else if ( c == '-' )
        {
            result.enqueue( new SubToken );
            i++;
        }
        else if ( c == '(' )
        {
            result.enqueue( new LParen<State> );
            i++;
        }
        else if ( c == ')' )
        {
            result.enqueue( new RParen<State> );
            i++;
        }
        else if ( c.isSpace() )
        {
            i++;
        }
        else if ( num_re.indexIn( str.mid( i ) ) == 0 )
        {
            result.enqueue( new ConstantToken( num_re.cap().toDouble() ) );
            i += num_re.matchedLength();
        }
        else if ( str.mid( i, 5 ) == "value" )
        {
            result.enqueue( new ValueToken() );
            i += 5;
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
/// Parse the unit conversion specification in the string.
/// 
/// \param [in] str The conversion string.
/// 
std::auto_ptr<Conversion> ParseConversion( const QString& str )
{
    State state( str );
    ParseExpr( state );

    if ( state.convstack.count() != 1 )
    {
        /// \todo I should probably figure out exactly when this occurs.
        throw Error( "Some kinda syntax error occurred." );
    }

    return std::auto_ptr<Conversion>( state.convstack.pop() );
}

} // namespace AutoUnits
