#ifndef AUTO_UNITS_TYPES_DIMENSION_ID_H
#define AUTO_UNITS_TYPES_DIMENSION_ID_H
//==============================================================================
/// \file AutoUnits/Types/DimensionId.h
/// 
/// Header file for the DimensionId type.
///
//==============================================================================

#include <QHash>

namespace AutoUnits
{

/// Uniquely identifies a dimension as a list of (Base unit, power) pairs.
/// For instance, acceleration would be [(Meter,1), (Second,-2)]
class DimensionId : public QHash<QString,int>
{
public:
    DimensionId() { }
    DimensionId( const QString& unit );

    bool operator==( const DimensionId& id ) const;

    DimensionId operator*( const DimensionId& rhs ) const;
    DimensionId operator/( const DimensionId& rhs ) const;
    DimensionId operator^( int exp ) const;
};

} // namespace AutoUnits

#endif // AUTO_UNITS_TYPES_DIMENSION_ID_H
