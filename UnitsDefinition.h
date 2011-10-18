#ifndef MATH_UNITS_AUTO_UNITS_UNITS_DEFINITION_H
#define MATH_UNITS_AUTO_UNITS_UNITS_DEFINITION_H
//==============================================================================
/// \file Math/Units/AutoUnits/UnitsDefinition.h
/// 
/// Header file for the AutoUnits::UnitsDefinition class.
///
//  Copyright 2011 AgLeader Technology, Inc.
//==============================================================================

#include <memory>
#include <QHash>
#include <QString>

namespace AutoUnits
{

class Unit;
class Dimension;

/// Uniquely identifies a dimension as a list of (Base unit, power) pairs.
/// For instance, acceleration would be [(Meter,1), (Second,-2)]
typedef QHash<QString,int> DimensionId;

//==============================================================================
/// An object that defines a unit system.
/// 
class UnitsDefinition
{
public:
    ~UnitsDefinition();
    static std::auto_ptr<UnitsDefinition> Create();

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
    UnitsDefinition();

    /// Maps name -> dimension
    QHash<QString,Dimension*> m_dimensions;

    /// Maps name -> unit
    QHash<QString,Unit*> m_units;
};

} // namespace AutoUnits

#endif // MATH_UNITS_AUTO_UNITS_UNITS_DEFINITION_H
