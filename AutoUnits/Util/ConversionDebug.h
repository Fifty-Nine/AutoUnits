#ifndef AUTO_UNITS_UTIL_CONVERSION_DEBUG_H
#define AUTO_UNITS_UTIL_CONVERSION_DEBUG_H
//==============================================================================
/// \file AutoUnits/Util/ConversionDebug.h
/// 
/// Debugging utilities for the conversion type.
///
//==============================================================================

class QDebug;

namespace AutoUnits {
namespace Conversions {

class Conversion;

QDebug operator<<( QDebug out, const Conversion& conv_p );

} // namespace Conversions
} // namespace AutoUnits

#endif // AUTO_UNITS_UTIL_CONVERSION_DEBUG_H
