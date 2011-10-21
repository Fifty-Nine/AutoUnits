#ifndef AUTO_UNITS_DERIVATION_PARSER_H
#define AUTO_UNITS_DERIVATION_PARSER_H
//==============================================================================
/// \file AutoUnits/DerivationParser.h
/// 
/// Header file for the derivation parsing functions.
///
//  Copyright 2011 AgLeader Technology, Inc.
//==============================================================================

#include "Types/DimensionId.h"

namespace AutoUnits
{

DimensionId ParseDerivation( const QString& str );

} // namespace AutoUnits

#endif // AUTO_UNITS_DERIVATION_PARSER_H
