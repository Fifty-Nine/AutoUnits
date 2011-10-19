//==============================================================================
/// \file Math/Units/AutoUnits/UnitSystem.cpp
/// 
/// Header file for the AutoUnits::UnitSystem class.
///
//  Copyright 2011 AgLeader Technology, Inc.
//==============================================================================

#include <cassert>

#include "Dimension.h"
#include "Unit.h"
#include "UnitSystem.h"

namespace 
{

//==============================================================================
/// Normalize the given name.
/// 
/// \param [in] name The name to normalize.
/// 
/// \return The normalized name.
/// 
QString NormalizeName( const QString& name )
{
    return name.trimmed().toUpper();
}

}

namespace AutoUnits
{

//==============================================================================
/// Destructor.
/// 
UnitSystem::~UnitSystem()
{
    qDeleteAll( m_dimensions );
    qDeleteAll( m_units );
}

//==============================================================================
/// Create a new units system.
/// 
/// \return A pointer to the created system.
///
std::auto_ptr<UnitSystem> UnitSystem::Create()
{
    return std::auto_ptr<UnitSystem>( new UnitSystem );
}


//==============================================================================
/// Immutable interface.
//==============================================================================

//==============================================================================
/// Get the dimensions in the units system.
/// 
/// \return The dimensions.
/// 
QList<const Dimension*> UnitSystem::Dimensions() const
{
    QList<const Dimension*> result;

    for ( QHash<QString,Dimension*>::const_iterator it = m_dimensions.begin(); 
        it != m_dimensions.end(); ++it )
    {
        result << it.value();
    }

    return result;
}

//==============================================================================
/// Get the units in the units system.
/// 
/// \return The units.
/// 
QList<const Unit*> UnitSystem::Units() const
{
    QList<const Unit*> result;

    for ( QHash<QString,Unit*>::const_iterator it = m_units.begin(); 
        it != m_units.end(); ++it )
    {
        result << it.value();
    }

    return result;
}

//==============================================================================
/// Get the dimension with the given identifier.
/// 
/// \param [in] dimension_id The identifier.
/// 
/// \return The dimension, or NULL if not present.
///
const Dimension *UnitSystem::GetDimension( const DimensionId& id ) const
{
    for ( QHash<QString,Dimension*>::const_iterator it = m_dimensions.begin();
        it != m_dimensions.end(); ++it )
    {
        if ( it.value()->Id() == id )
        {
            return it.value();
        }
    }

    return NULL;
}

//==============================================================================
/// Get the dimension with the given name.
/// 
/// \param [in] name The name of the dimension.
/// 
/// \return The dimension, or NULL if not present.
/// 
const Dimension *UnitSystem::GetDimension( const QString& name ) const
{
    QHash<QString,Dimension*>::const_iterator it = 
        m_dimensions.find( NormalizeName( name ) );

    return ( it != m_dimensions.end() ) ? it.value() : NULL;
}

//==============================================================================
/// Get the unit with the given name.
/// 
/// \param [in] name The name of the dimension.
/// 
/// \return The unit, or NULL if not present.
/// 
const Unit *UnitSystem::GetUnit( const QString& name ) const
{
    QHash<QString,Unit*>::const_iterator it = 
        m_units.find( NormalizeName( name ) );

    return ( it != m_units.end() ) ? it.value() : NULL;
}

//==============================================================================
// Mutable interface (used only during parsing).
//==============================================================================

//==============================================================================
/// Get the dimensions in the unit system.
/// 
/// \return The list of dimensions.
/// 
QList<Dimension*> UnitSystem::Dimensions()
{
    return m_dimensions.values();
}

//==============================================================================
/// Add a new dimension to the unit system.
/// 
/// \param [in] name The name of the dimension.
/// \param [in] id The ID of the dimension.
/// 
/// \return The dimension.
/// 
Dimension *UnitSystem::NewDimension( 
    const QString& name, const DimensionId& id )
{
    // \todo Throw exception.
    assert( !GetDimension( name ) );
    assert( !GetDimension( id ) );

    Dimension *dim_p = new Dimension( name, id );

    m_dimensions.insert( NormalizeName( name ), dim_p );

    return dim_p;
}

//==============================================================================
/// Get the dimension with the given name.
/// 
/// \param [in] name The name of the dimension.
/// 
/// \return The dimension, or NULL if not present.
/// 
Dimension *UnitSystem::GetDimension( const QString& name )
{
    QHash<QString,Dimension*>::const_iterator it = 
        m_dimensions.find( NormalizeName( name ) );

    return ( it != m_dimensions.end() ) ? it.value() : NULL;
}

//==============================================================================
/// Get the dimension with the given id.
/// 
/// \param [in] id The id of the dimension.
/// 
/// \return The dimension, or NULL if not present.
/// 
Dimension *UnitSystem::GetDimension( const DimensionId& id )
{
    for ( QHash<QString,Dimension*>::const_iterator it = m_dimensions.begin(); 
        it != m_dimensions.end(); ++it )
    {
        if ( it.value()->Id() == id )
        {
            return it.value();
        }
    }

    return NULL;
}

//==============================================================================
/// Get the units in the units system.
/// 
/// \return The units.
/// 
QList<Unit*> UnitSystem::Units()
{
    return m_units.values();
}

//==============================================================================
/// Add a new unit to the unit system.
/// 
/// \param [in] name The unit name.
/// \param [in] dim_p The unit dimension.
/// 
/// \return The new unit.
/// 
Unit *UnitSystem::NewUnit( const QString& name, Dimension *dim_p )
{
    assert( !GetUnit( name ) );

    Unit *unit_p = new Unit( name, dim_p );

    m_units.insert( NormalizeName( name ), unit_p );

    return unit_p;
}

//==============================================================================
/// Get the unit with the given name.
/// 
/// \param [in] name The name of the unit.
/// 
/// \return A pointer to the unit, or NULL if not present.
/// 
Unit *UnitSystem::GetUnit( const QString& name )
{
    QHash<QString,Unit*>::const_iterator it = 
        m_units.find( NormalizeName( name ) );

    return ( it != m_units.end() ) ? it.value() : NULL;
}

//==============================================================================
/// Constructor.
/// 
UnitSystem::UnitSystem()
{
}

} // namespace AutoUnits
