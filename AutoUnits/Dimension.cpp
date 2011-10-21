//==============================================================================
/// \file AutoUnits/Dimension.h
/// 
/// Header file for the AutoUnits::Dimension class.
///
//  Copyright 2011 AgLeader Technology, Inc.
//==============================================================================

#include <cassert>

#include "Dimension.h"

namespace AutoUnits
{

//==============================================================================
/// Get the name of the dimension.
/// 
/// \return The name.
/// 
QString Dimension::Name() const
{
    return m_name;
}

//==============================================================================
/// Get the ID of the dimension.
/// 
/// \return The ID.
/// 
DimensionId Dimension::Id() const
{
    return m_id;
}

//==============================================================================
/// Determine whether the dimension is a derived dimension.
/// 
/// \return True if the dimension is derived from other dimensions.
///
bool Dimension::IsDerived() const
{
    return m_id.count() > 1;
}


//==============================================================================
/// Get the base unit for the dimension.
/// 
/// \return The base unit.
/// 
const Unit *Dimension::GetBaseUnit() const
{
    return m_base_unit_p;
}

//==============================================================================
/// Get the units in the dimension.
///
/// \return The list of all the units in the dimension.
/// 
QList<const Unit*> Dimension::Units() const
{
    QList<const Unit*> result;

    for ( int i = 0; i < m_units.count(); ++i )
    {
        result << m_units[i];
    }
    return result;
}

//==============================================================================
/// Get the base unit for the dimension.
/// 
/// \return The base unit.
/// 
Unit *Dimension::GetBaseUnit() 
{
    return m_base_unit_p;
}

//==============================================================================
/// Set the base unit for the dimension.
/// 
/// \param [in] unit_p The new base unit.
/// 
void Dimension::SetBaseUnit( Unit *unit_p )
{
    assert( !m_base_unit_p );
    assert( m_units.contains( unit_p ) );
    m_base_unit_p = unit_p;
}

//==============================================================================
/// Add the unit to the dimension.
///
/// \param [in] unit_p The unit to add.
/// 
void Dimension::AddUnit( Unit *unit_p )
{
    assert( !m_units.contains( unit_p ) );
    m_units.append( unit_p );
}

//==============================================================================
/// Get the list of units in the dimension.
/// 
/// \return The list.
/// 
QList<Unit*> Dimension::Units()
{
    return m_units;
}

//==============================================================================
/// Constructor.
/// 
/// \param [in] name The name of the dimension.
/// \param [in] id The ID of the dimension.
/// 
Dimension::Dimension( const QString& name, const DimensionId& id ) : 
    m_name( name ), m_id( id ), m_base_unit_p( NULL )
{
}

} // namespace AutoUnits
