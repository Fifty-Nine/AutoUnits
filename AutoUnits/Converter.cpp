//==============================================================================
/// \file AutoUnits/Converter.cpp
/// 
/// Source file for the AutoUnits::Converter class.
///
//  Copyright 2011 AgLeader Technology, Inc.
//==============================================================================

#include <cassert>

#include "Converter.h"
#include "Dimension.h"
#include "Types/Conversion.h"
#include "Unit.h"
#include "UnitSystem.h"

namespace AutoUnits
{


namespace
{

//==============================================================================
/// Compute the conversion factor from \c from to \c to.
/// 
/// \param [in] system_p The unit system.
/// \param [in] from The source unit.
/// \param [in] to The destination unit.
/// 
std::auto_ptr<Conversion> Compute( 
    const UnitSystem *system_p, const QString& from, const QString& to )
{
    (void)system_p; (void)from; (void)to;
    return std::auto_ptr<Conversion>( new Conversions::Value() );
}

}

//==============================================================================
/// Constructor.
///
/// \param [in] system_p The unit system.
/// 
/// \note This assumes that the unit system doesn't change for the lifetime
///       of the converter. 
/// 
/// 
Converter::Converter( const UnitSystem *system_p ) : 
    m_system_p( system_p )
{
}

//==============================================================================
/// Destructor.
/// 
Converter::~Converter()
{
    qDeleteAll( m_cache );
}

//==============================================================================
/// Check whether a conversion exists from the given unit to the other unit.
/// 
/// \param [in] from The unit to convert from.
/// \param [in] to The unit to convert to.
/// 
/// \return True if the conversion is possible.
/// 
bool Converter::CanConvert( const QString& from, const QString& to ) const
{
    if ( m_cache.contains( CacheKey( from, to ) ) )
    {
        return true;
    }

    const Unit *from_p = m_system_p->GetUnit( from );
    const Unit *to_p = m_system_p->GetUnit( to );

    if ( to_p->GetDimension() == from_p->GetDimension() )
    {
        m_cache.insert( 
            CacheKey( from, to ), Compute( m_system_p, from, to ).release() );
        return true;
    }
    return false;
}

//==============================================================================
/// Get the converted value of the given unit in the new units.
/// 
/// \param [in] from The source unit type.
/// \param [in] to The desired unit type.
/// \param [in] value The source value.
/// 
/// \return The converted value.
/// 
double Converter::Convert( 
    const QString& from, const QString& to, double value ) const
{
    assert( CanConvert( from, to ) );

    CacheKey key( from, to );
    Cache::const_iterator it = m_cache.find( key );
    if ( it != m_cache.end() )
    {
        return it.value()->Eval( value );
    }

    Conversion *conv_p = Compute( m_system_p, from, to ).release();

    m_cache.insert( key, conv_p );

    return conv_p->Eval( value );
}

} // namespace AutoUnits

