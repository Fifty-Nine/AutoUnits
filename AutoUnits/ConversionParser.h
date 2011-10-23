#ifndef AUTO_UNITS_CONVERSION_PARSER_H
#define AUTO_UNITS_CONVERSION_PARSER_H
//==============================================================================
/// \file AutoUnits/ConversionParser.h
/// 
/// Header file for the conversion function parser.
///
//  Copyright 2011 AgLeader Technology, Inc.
//==============================================================================

namespace YAML { class Node; }

namespace AutoUnits
{

class Unit;

//==============================================================================
/// Parse the unit conversion specification in the given YAML node and store it
/// in the given unit. 
/// 
void ParseConversion( YAML::Node& node, Unit *unit_p );

} // namespace AutoUnits

#endif // AUTO_UNITS_CONVERSION_PARSER_H
