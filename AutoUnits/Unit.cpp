//==============================================================================
/// \file Math/Units/AutoUnits/Unit.cpp
/// 
/// Header file for the Unit class.
///
//  Copyright 2011 AgLeader Technology, Inc.
//==============================================================================

#include "Dimension.h"
#include "Unit.h"

namespace AutoUnits
{

//==============================================================================
/// Get the name of the unit.
/// 
/// \return The name.
/// 
QString Unit::Name() const
{
    return m_name;
}

//==============================================================================
/// Get the dimension for the unit.
/// 
/// \return The dimension.
/// 
const Dimension *Unit::GetDimension() const
{
    return m_dim_p;
}

//==============================================================================
/// Get the conversion from the unit to the base unit.
/// 
/// \return The text of the conversion.
/// 
QString Unit::Conversion() const
{
    return m_conversion;
}

//==============================================================================
/// Get the dimension for the unit.
/// 
/// \return The dimension.
/// 
Dimension *Unit::GetDimension() 
{
    return m_dim_p;
}

//==============================================================================
/// Set the conversion from the unit to the base unit.
/// 
/// \param [in] conversion The new conversion.
/// 
void Unit::SetConversion( const QString& conversion ) 
{
    m_conversion = conversion;
}

//==============================================================================
/// Constructor.
/// 
/// \param [in] name The name of the unit.
/// \param [in] dimension_p The dimension for the unit.
/// 
Unit::Unit( const QString& name, Dimension *dimension_p ) : 
    m_name( name ), m_dim_p( dimension_p ), m_conversion( "1.0" )
{
    m_dim_p->AddUnit( this );
}

} // namespace AutoUnits
