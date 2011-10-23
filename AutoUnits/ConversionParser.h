#ifndef AUTO_UNITS_CONVERSION_PARSER_H
#define AUTO_UNITS_CONVERSION_PARSER_H
//==============================================================================
/// \file AutoUnits/ConversionParser.h
/// 
/// Header file for the conversion function parser.
///
//==============================================================================

#include <memory>

#include <QString>

namespace AutoUnits
{

namespace Conversions { class Conversion; }

std::auto_ptr<Conversions::Conversion> ParseConversion( const QString& str );

} // namespace AutoUnits

#endif // AUTO_UNITS_CONVERSION_PARSER_H
