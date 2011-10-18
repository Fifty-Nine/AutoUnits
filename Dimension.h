#ifndef MATH_UNITS_AUTO_UNITS_DIMENSION_H
#define MATH_UNITS_AUTO_UNITS_DIMENSION_H
//==============================================================================
/// \file Math/Units/AutoUnits/Dimension.h
/// 
/// Header file for the AutoUnits::Dimension class.
///
//  Copyright 2011 AgLeader Technology, Inc.
//==============================================================================

#include <QHash>
#include <QString>

namespace AutoUnits
{

/// Uniquely identifies a dimension as a list of (Base unit, power) pairs.
/// For instance, acceleration would be [(Meter,1), (Second,-2)]
typedef QHash<QString,int> DimensionId;

class Unit;

//==============================================================================
/// An object that defines a dimension in a unit system.
class Dimension 
{
public:
    
    //==========================================================================
    /// Immutable interface.
    //==========================================================================
    QString Name() const;
    DimensionId Id() const;

    const Unit *GetBaseUnit() const;
    QList<const Unit*> Units() const;

    //==========================================================================
    /// Mutable interface.
    //==========================================================================
    Unit *GetBaseUnit();
    void SetBaseUnit( Unit *unit_p );
    void AddUnit( Unit *unit_p );
    QList<Unit*> Units();

private:
    Dimension( const QString& name, const DimensionId& id );

    friend class UnitsDefinition;

    /// The name of the dimension.
    QString m_name;

    /// The unique identifier of the dimension.
    DimensionId m_id;

    /// The base unit.
    Unit *m_base_unit_p;

    /// The list of all units in the dimension.
    QList<Unit*> m_units;
};

} // namespace AutoUnits

#endif // MATH_UNITS_AUTO_UNITS_DIMENSION_H
