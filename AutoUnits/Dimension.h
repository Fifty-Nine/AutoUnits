#ifndef AUTO_UNITS_DIMENSION_H
#define AUTO_UNITS_DIMENSION_H
//==============================================================================
/// \file AutoUnits/Dimension.h
/// 
/// Header file for the AutoUnits::Dimension class.
///
//==============================================================================

#include <QHash>
#include <QString>

#include "Types/DimensionId.h"

namespace AutoUnits
{

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
    bool IsDerived() const;

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

    friend class UnitSystem;

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
