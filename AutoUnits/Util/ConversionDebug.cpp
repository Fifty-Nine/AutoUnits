//==============================================================================
/// \file AutoUnits/Util/ConversionDebug.cpp
/// 
/// Source file for the conversion debug utilities.
///
//==============================================================================

#include <QDebug>

#include "Types/Conversion.h"
#include "Util/ConversionDebug.h"

namespace AutoUnits {
namespace Conversions {

QDebug operator<<( QDebug out, const Conversion& conv )
{
    return out << conv.ToString();
}

} // namespace Conversions
} // namespace AutoUnits
