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
    QString GetName() const;

    Dimension *GetDimension();
    const Dimension *GetDimension() const;

private:
    Unit( const QString& name, Dimension *dimension_p );

    friend class UnitsDefinition;

    Dimension *m_dim_p;
};

} // namespace AutoUnits

