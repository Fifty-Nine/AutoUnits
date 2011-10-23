#ifndef AUTO_UNITS_CONVERTER_H
#define AUTO_UNITS_CONVERTER_H
//==============================================================================
/// \file AutoUnits/Converter.h
/// 
/// Header file for the AutoUnits::Converter class.
///
//==============================================================================

#include <QHash>
#include <QString>

namespace AutoUnits
{

namespace Conversions { class Conversion; }
using Conversions::Conversion;
class UnitSystem;

//==============================================================================
/// A class to compute and cache runtime conversions between units.
/// 
class Converter
{
public:
    Converter( const UnitSystem *system_p );
    ~Converter();

    bool CanConvert( const QString& from, const QString& to ) const;
    double Convert( const QString& from, const QString& to, double value ) 
        const;
    const Conversion *GetConversion( const QString& from, const QString& to ) 
        const;

private:
    /// Not implemented. This is here just as a reminder that system_p needs
    /// to be immutable for the lifetime of the converter.
    Converter( UnitSystem* );

    /// Our unit system.
    const UnitSystem *m_system_p;

    /// Our cached conversions.
    typedef QPair<QString,QString> CacheKey;
    typedef QHash<CacheKey, Conversion*> Cache;
    mutable Cache m_cache;
};

}

#endif // AUTO_UNITS_CONVERTER_H
