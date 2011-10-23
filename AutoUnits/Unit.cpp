//==============================================================================
/// \file AutoUnits/Unit.cpp
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
/// Get the conversion to the base unit.
/// 
/// \return The conversion.
/// 
const Conversion *Unit::ToBase() const
{
    return m_to_base_p.get();
}

//==============================================================================
/// Get the conversion to the base unit.
/// 
/// \return The conversion.
/// 
Conversion *Unit::ToBase() 
{
    return m_to_base_p.get();
}

//==============================================================================
/// Get the conversion from the base unit.
/// 
/// \return The conversion.
/// 
const Conversion *Unit::FromBase() const
{
    return m_from_base_p.get();
}

//==============================================================================
/// Get the conversion from the base unit.
/// 
/// \return The conversion.
/// 
Conversion *Unit::FromBase() 
{
    return m_from_base_p.get();
}

//==============================================================================
/// Set the to-base conversion.
/// 
/// \param [in] conv_p The conversion.
/// 
void Unit::SetToBase( std::auto_ptr<Conversion> conv_p )
{
    m_to_base_p.reset( conv_p.release() );
}

//==============================================================================
/// Set the from-base conversion.
/// 
/// \param [in] conv_p The conversion.
/// 
void Unit::SetFromBase( std::auto_ptr<Conversion> conv_p )
{
    m_from_base_p.reset( conv_p.release() );
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
/// Constructor.
/// 
/// \param [in] name The name of the unit.
/// \param [in] dimension_p The dimension for the unit.
/// 
Unit::Unit( const QString& name, Dimension *dimension_p ) : 
    m_name( name ), m_dim_p( dimension_p ), 
    m_to_base_p( new Conversions::Value() ), 
    m_from_base_p( new Conversions::Value() )
{
    m_dim_p->AddUnit( this );
}

//==============================================================================
/// Constructor.
/// 
/// \param [in] name The name of the unit.
/// \param [in] dimension_p The dimension for the unit.
/// \param [in] to_base_p The conversion to the base unit.
/// \param [in] from_base_p The conversion from the base unit.
/// 
Unit::Unit( const QString& name, Dimension *dimension_p, 
    std::auto_ptr<Conversion> to_base_p, 
    std::auto_ptr<Conversion> from_base_p ) : 
    m_name( name ), m_dim_p( dimension_p ), 
    m_to_base_p( to_base_p ), m_from_base_p( from_base_p )
{
    m_dim_p->AddUnit( this );
}

} // namespace AutoUnits
