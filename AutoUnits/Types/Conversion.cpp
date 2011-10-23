//==============================================================================
/// \file AutoUnits/Types/Conversion.cpp
/// 
/// Source file for the conversion type.
///
//  Copyright 2011 AgLeader Technology, Inc.
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
std::auto_ptr<Conversion> Conversion::ScaleFactor( double factor )
{
    std::auto_ptr<Conversion> lhs_p( new Constant( factor ) );
    std::auto_ptr<Conversion> rhs_p( new Value );
    return std::auto_ptr<Conversion>( new MultOp( lhs_p, rhs_p ) );
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
Conversion::AutoPtr Constant::Compose( Conversion::AutoPtr ) const
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
/// \param [in] value_p The value to compose.
/// 
/// \return The composed value.
/// 
Conversion::AutoPtr Value::Compose( Conversion::AutoPtr value_p ) const
{
    return value_p;
}

//==============================================================================
/// Constructor.
/// 
/// \param [in] lhs The left hand side.
/// \param [in] rhs The right hand side.
/// 
AddOp::AddOp( 
    std::auto_ptr<Conversion> lhs_p, std::auto_ptr<Conversion> rhs_p ) : 
    BinOp( lhs_p, rhs_p )
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
/// \param [in] value_p The value to compose with.
/// 
/// \return The composed conversion.
/// 
Conversion::AutoPtr AddOp::Compose( Conversion::AutoPtr value_p ) const
{
    Conversion::AutoPtr left_p( m_lhs_p->Compose( value_p->Clone() ) );
    Conversion::AutoPtr right_p( m_rhs_p->Compose( value_p ) );

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
/// \param [in] lhs The left hand side.
/// \param [in] rhs The right hand side.
/// 
SubOp::SubOp( 
    std::auto_ptr<Conversion> lhs_p, std::auto_ptr<Conversion> rhs_p ) : 
    BinOp( lhs_p, rhs_p )
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
/// \param [in] value_p The value to compose with.
/// 
/// \return The composed conversion.
/// 
Conversion::AutoPtr SubOp::Compose( Conversion::AutoPtr value_p ) const
{
    Conversion::AutoPtr left_p( m_lhs_p->Compose( value_p->Clone() ) );
    Conversion::AutoPtr right_p( m_rhs_p->Compose( value_p ) );

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
MultOp::MultOp( 
    std::auto_ptr<Conversion> lhs_p, std::auto_ptr<Conversion> rhs_p ) :
    BinOp( lhs_p, rhs_p )
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
/// \param [in] value_p The value to compose with.
/// 
/// \return The composed conversion.
/// 
Conversion::AutoPtr MultOp::Compose( Conversion::AutoPtr value_p ) const
{
    Conversion::AutoPtr left_p( m_lhs_p->Compose( value_p->Clone() ) );
    Conversion::AutoPtr right_p( m_rhs_p->Compose( value_p ) );

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
DivOp::DivOp( 
    std::auto_ptr<Conversion> lhs_p, std::auto_ptr<Conversion> rhs_p ) : 
    BinOp( lhs_p, rhs_p )
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
/// \param [in] value_p The value to compose with.
/// 
/// \return The composed conversion.
/// 
Conversion::AutoPtr DivOp::Compose( Conversion::AutoPtr value_p ) const
{
    Conversion::AutoPtr left_p( m_lhs_p->Compose( value_p->Clone() ) );
    Conversion::AutoPtr right_p( m_rhs_p->Compose( value_p ) );

    if ( left_p->IsConstant() && right_p->IsConstant() )
    {
        return Conversion::AutoPtr( new Constant( 
            left_p->Eval( 1.0 ) / right_p->Eval( 1.0 ) ) );
    }

    return Conversion::AutoPtr( new DivOp( left_p, right_p ) );
}

} // namespace Conversions

} // namespace AutoUnits
