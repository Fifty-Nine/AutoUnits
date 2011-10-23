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
double Constant::Eval( double ) const
{
    return m_value; 
}

//==============================================================================
/// Test whether the node is a constant.
/// 
bool Constant::IsConstant() const 
{
    return true;
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

} // namespace Conversions

} // namespace AutoUnits
