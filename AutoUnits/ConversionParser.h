#ifndef AUTO_UNITS_CONVERSION_PARSER_H
#define AUTO_UNITS_CONVERSION_PARSER_H
//==============================================================================
/// \file AutoUnits/ConversionParser.h
/// 
/// Header file for the conversion function parser.
///
//  Copyright 2011 AgLeader Technology, Inc.
//==============================================================================

#include <memory>

#include <QString>

namespace AutoUnits
{

namespace Conversions { class Conversion; }

//==============================================================================
/// Parse the conversion string.
/// 
/// \return The parsed conversion.
/// 
std::auto_ptr<Conversions::Conversion> ParseConversion( const QString& str );

} // namespace AutoUnits

#endif // AUTO_UNITS_CONVERSION_PARSER_H
