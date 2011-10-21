//==============================================================================
/// \file AutoUnits/Unit.h
/// 
/// Header file for the Unit class.
///
//  Copyright 2011 AgLeader Technology, Inc.
//==============================================================================

#include <QString>

namespace AutoUnits
{

class Dimension;

//==============================================================================
/// An object that specifies a base unit in a unit system.
/// 
class Unit
{
public:
    //==========================================================================
    /// Immutable interface.
    //==========================================================================
    QString Name() const;
    const Dimension *GetDimension() const;
    QString Conversion() const;

    //==========================================================================
    /// Mutable interface.
    //==========================================================================
    Dimension *GetDimension();
    void SetConversion( const QString& conversion );

private:
    Unit( const QString& name, Dimension *dimension_p );

    friend class UnitSystem;

    /// The unit's name.
    QString m_name;
    /// The unit's dimension.
    Dimension *m_dim_p;
    /// The unit's conversion specification.
    QString m_conversion;
};

} // namespace AutoUnits

