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
class BaseUnit;
class Dimension;
class DerivedUnit;
class ConvertedUnit;

/// Uniquely identifies a dimension as a list of (Base unit, power) pairs.
/// For instance, acceleration would be [(Meter,1), (Second,-2)]
typedef QHash<QString,int> DimensionId;

//==============================================================================
/// An object that defines a unit system.
/// 
class UnitsDefinition
{
public:
    static std::auto_ptr<UnitsDefinition> Create();

    //==========================================================================
    /// Immutable interface.
    //==========================================================================
    QList<const Dimension*> Dimensions() const;
    QList<const BaseUnit*> BaseUnits() const;
    QList<const DerivedUnit*> DerivedUnits() const;
    QList<ConvertedUnit*> ConvertedUnits() const;
    const Dimension* GetDimension( const DimensionId& id ) const;
    const Dimension *GetDimension( const QString& name ) const;
    const BaseUnit *GetBaseUnit( const QString& name ) const;
    const DerivedUnit *GetDerivedUnit( const QString& name ) const;
    const ConvertedUnit *GetConvertedUnit( const QString& name ) const;

    //==========================================================================
    // Mutable interface (used only during parsing).
    //==========================================================================
    QList<Dimension*> Dimensions();
    Dimension *NewDimension( const QString& name, const DimensionId& id );
    Dimension *GetDimension( const QString& name );
    Dimension* GetDimension( const DimensionId& id );

    QList<BaseUnit*> BaseUnits();
    BaseUnit *NewBaseUnit( const QString& name );
    BaseUnit *GetBaseUnit( const QString& name );

    QList<DerivedUnit*> DerivedUnits();
    DerivedUnit *NewDerivedUnit( const QString& name, Dimension *dim_p );
    DerivedUnit *GetDerivedUnit( const QString& name );

    QList<const ConvertedUnit*> ConvertedUnits();
    ConvertedUnit *NewConvertedUnit( const QString& name );
    ConvertedUnit *GetConvertedUnit( const QString& name );

private:
    UnitsDefinition();

    class Data;
    /// Our private data.
    const std::auto_ptr<Data> m_data_p;
};

} // namespace AutoUnits

#endif // MATH_UNITS_AUTO_UNITS_UNITS_DEFINITION_H
