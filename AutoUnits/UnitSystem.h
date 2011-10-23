#ifndef AUTO_UNITS_UNIT_SYSTEM_H
#define AUTO_UNITS_UNIT_SYSTEM_H
//==============================================================================
/// \file AutoUnits/UnitSystem.h
/// 
/// Header file for the AutoUnits::UnitSystem class.
///
//==============================================================================

#include <memory>
#include <QHash>
#include <QString>

#include "Types/DimensionId.h"

namespace AutoUnits
{

class Unit;
class Dimension;

//==============================================================================
/// An object that defines a unit system.
/// 
class UnitSystem
{
public:
    ~UnitSystem();
    static std::auto_ptr<UnitSystem> Create();

    //==========================================================================
    /// Immutable interface.
    //==========================================================================
    QList<const Dimension*> Dimensions() const;
    QList<const Unit*> Units() const;
    const Dimension* GetDimension( const DimensionId& id ) const;
    const Dimension *GetDimension( const QString& name ) const;
    const Unit *GetUnit( const QString& name ) const;

    //==========================================================================
    // Mutable interface (used only during parsing).
    //==========================================================================
    QList<Dimension*> Dimensions();
    Dimension *NewDimension( const QString& name, const DimensionId& id );
    Dimension *GetDimension( const QString& name );
    Dimension* GetDimension( const DimensionId& id );

    QList<Unit*> Units();
    Unit *NewUnit( const QString& name, Dimension *dim_p );
    Unit *GetUnit( const QString& name );

private:
    UnitSystem();

    /// Maps name -> dimension
    QHash<QString,Dimension*> m_dimensions;

    /// Maps name -> unit
    QHash<QString,Unit*> m_units;
};

} // namespace AutoUnits

#endif // AUTO_UNITS_UNIT_SYSTEM_H
