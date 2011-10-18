//==============================================================================
/// \file Math/Units/AutoUnits/Unit.h
/// 
/// Header file for the Unit class.
///
//  Copyright 2011 AgLeader Technology, Inc.
//==============================================================================

namespace AutoUnits
{

//==============================================================================
/// An object that specifies a base unit in a unit system.
/// 
class Unit
{
public:
    //==========================================================================
    /// Immutable interface.
    //==========================================================================
    QString GetName() const;
    const Dimension *GetDimension() const;
    QString ConversionToBase() const;

    //==========================================================================
    /// Mutable interface.
    //==========================================================================
    Dimension *GetDimension();
    void SetConversionToBase( const QString& conversion );

private:
    Unit( const QString& name, Dimension *dimension_p );

    friend class UnitsDefinition;

    /// The unit's name.
    QString m_name;
    /// The unit's dimension.
    Dimension *m_dim_p;
    /// The unit's conversion specification.
    QString m_conversion;
};

} // namespace AutoUnits

