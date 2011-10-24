//==============================================================================
/// \file AutoUnits/Types/Conversion.cpp
/// 
/// Source file for the conversion type.
///
//==============================================================================

#include "Types/Conversion.h"

namespace AutoUnits
{

namespace Conversions
{

//==============================================================================
/// Constructor.
/// 
Conversion::Conversion()
{
}

//==============================================================================
/// Destructor.
/// 
Conversion::~Conversion()
{
}

//==============================================================================
/// Construct a simple scaling factor conversion.
/// 
/// \param [in] factor The scaling factor.
/// 
Conversion::AutoPtr Conversion::ScaleFactor( double factor )
{
    Conversion::AutoPtr lhs_p( new Constant( factor ) );
    Conversion::AutoPtr rhs_p( new Value );
    return Conversion::AutoPtr( new MultOp( lhs_p, rhs_p ) );
}

//==============================================================================
/// Constructor.
/// 
/// \param [in] value The constant's value.
/// 
Constant::Constant( double value ) : 
    m_value( value ) 
{ 
}
    
//==============================================================================
/// Get the constant's value.
/// 
/// \return The value.
/// 
double Constant::Value() const 
{ 
    return m_value; 
}

//==============================================================================
/// Set the constant's value.
/// 
/// \param [in] value The new value.
/// 
void Constant::SetValue( double value ) 
{ 
    m_value = value; 
}

//==============================================================================
/// Evaluate the conversion for the given value.
/// 
/// \return The value.
/// 
double Constant::Eval( double ) const
{
    return m_value; 
}

//==============================================================================
/// Compose the conversion with the given conversion.
/// 
/// \return The composed conversion.
/// 
Conversion::AutoPtr Constant::Compose( const Conversion& ) const
{
    return Conversion::AutoPtr( new Constant( *this ) );
}

//==============================================================================
/// Test whether the node is a constant.
/// 
bool Constant::IsConstant() const 
{
    return true;
}

//==============================================================================
/// Evaluate the value.
/// 
/// \param [in] value The value.
///
/// \return The value.
///
double Value::Eval( double value ) const
{
    return value;
}

//==============================================================================
/// Compose the value with the given expression.
/// 
/// \param [in] value The value to compose with.
/// 
/// \return The composed value.
/// 
Conversion::AutoPtr Value::Compose( const Conversion& value ) const
{
    return value.Clone();
}

//==============================================================================
/// Constructor.
/// 
/// \param [in] lhs_p The left hand side.
/// \param [in] rhs_p The right hand side.
/// 
AddOp::AddOp( Conversion::AutoPtr lhs_p, Conversion::AutoPtr rhs_p ) : 
    Private::BinOp<AddOp>( lhs_p, rhs_p )
{ 
}

//==============================================================================
/// Evaluate the conversion for the given value.
/// 
/// \param [in] value The value to convert.
/// 
/// \return The converted value.
/// 
double AddOp::Eval( double value ) const
{
    return m_lhs_p->Eval( value ) + m_rhs_p->Eval( value );
}

//==============================================================================
/// Compose the conversion for the given value.
/// 
/// \param [in] value The value to compose with.
/// 
/// \return The composed conversion.
/// 
Conversion::AutoPtr AddOp::Compose( const Conversion& value ) const
{
    Conversion::AutoPtr left_p( m_lhs_p->Compose( value ) );
    Conversion::AutoPtr right_p( m_rhs_p->Compose( value ) );

    if ( left_p->IsConstant() && right_p->IsConstant() )
    {
        return Conversion::AutoPtr( new Constant( 
            left_p->Eval( 1.0 ) + right_p->Eval( 1.0 ) ) );
    }

    return Conversion::AutoPtr( new AddOp( left_p, right_p ) );
}

//==============================================================================
/// Constructor.
/// 
/// \param [in] lhs_p The left hand side.
/// \param [in] rhs_p The right hand side.
/// 
SubOp::SubOp( Conversion::AutoPtr lhs_p, Conversion::AutoPtr rhs_p ) : 
    Private::BinOp<SubOp>( lhs_p, rhs_p )
{
}

//==============================================================================
/// Evaluate the conversion for the given value.
/// 
/// \param [in] value The value to convert.
/// 
/// \return The converted value.
/// 
double SubOp::Eval( double value ) const
{
    return m_lhs_p->Eval( value ) - m_rhs_p->Eval( value );
}

//==============================================================================
/// Compose the conversion for the given value.
/// 
/// \param [in] value The value to compose with.
/// 
/// \return The composed conversion.
/// 
Conversion::AutoPtr SubOp::Compose( const Conversion& value ) const
{
    Conversion::AutoPtr left_p( m_lhs_p->Compose( value ) );
    Conversion::AutoPtr right_p( m_rhs_p->Compose( value ) );

    if ( left_p->IsConstant() && right_p->IsConstant() )
    {
        return Conversion::AutoPtr( new Constant( 
            left_p->Eval( 1.0 ) - right_p->Eval( 1.0 ) ) );
    }

    return Conversion::AutoPtr( new SubOp( left_p, right_p ) );
}

//==============================================================================
/// Constructor.
/// 
/// \param [in] lhs_p The left-hand side of the multiplication.
/// \param [in] rhs_p The right-hand side of the multiplication.
/// 
MultOp::MultOp( Conversion::AutoPtr lhs_p, Conversion::AutoPtr rhs_p ) :
    Private::BinOp<MultOp>( lhs_p, rhs_p )
{
}

//==============================================================================
/// Evaluate the conversion for the given value.
/// 
/// \param [in] value The value to convert.
/// 
/// \return The converted value.
/// 
double MultOp::Eval( double value ) const
{
    return m_lhs_p->Eval( value ) * m_rhs_p->Eval( value );
}

//==============================================================================
/// Compose the conversion for the given value.
/// 
/// \param [in] value The value to compose with.
/// 
/// \return The composed conversion.
/// 
Conversion::AutoPtr MultOp::Compose( const Conversion& value ) const
{
    Conversion::AutoPtr left_p( m_lhs_p->Compose( value ) );
    Conversion::AutoPtr right_p( m_rhs_p->Compose( value ) );

    if ( left_p->IsConstant() && right_p->IsConstant() )
    {
        return Conversion::AutoPtr( new Constant( 
            left_p->Eval( 1.0 ) * right_p->Eval( 1.0 ) ) );
    }

    return Conversion::AutoPtr( new MultOp( left_p, right_p ) );
}

//==========================================================================
/// Constructor.
/// 
DivOp::DivOp( Conversion::AutoPtr lhs_p, Conversion::AutoPtr rhs_p ) : 
    Private::BinOp<DivOp>( lhs_p, rhs_p )
{
}

//==============================================================================
/// Evaluate the conversion for the given value.
/// 
/// \param [in] value The value to convert.
/// 
/// \return The converted value.
/// 
double DivOp::Eval( double value ) const
{
    return m_lhs_p->Eval( value ) / m_rhs_p->Eval( value );
}

//==============================================================================
/// Compose the conversion for the given value.
/// 
/// \param [in] value The value to compose with.
/// 
/// \return The composed conversion.
/// 
Conversion::AutoPtr DivOp::Compose( const Conversion& value ) const
{
    Conversion::AutoPtr left_p( m_lhs_p->Compose( value ) );
    Conversion::AutoPtr right_p( m_rhs_p->Compose( value ) );

    if ( left_p->IsConstant() && right_p->IsConstant() )
    {
        return Conversion::AutoPtr( new Constant( 
            left_p->Eval( 1.0 ) / right_p->Eval( 1.0 ) ) );
    }

    return Conversion::AutoPtr( new DivOp( left_p, right_p ) );
}

//==============================================================================
/// Convenience function to compose two conversions.
/// 
/// \param [in] f The f(x) conversion.
/// \param [in] g The g(x) conversion.
/// 
/// \return A conversion for f(g(x)).
/// 
Conversion::AutoPtr Compose( const Conversion& f, const Conversion& g )
{
    return f.Compose( g );
}

//==============================================================================
/// Convenience function to compose two conversions.
/// 
/// \param [in] f The f(x) conversion.
/// \param [in] g The g(x) conversion.
/// 
/// \return A conversion for f(g(x)).
/// 
Conversion::AutoPtr Compose( 
    const Conversion::AutoPtr& f, const Conversion::AutoPtr& g )
{
    return f->Compose( *g );
}

} // namespace Conversions

} // namespace AutoUnits
