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

//==============================================================================
/// Error thrown when a problem is encountered in a derivation.
/// 
class DerivationError
{
public:
    DerivationError( const QString& description );

    operator QString() const;

    const QString m_desc;
};

DimensionId ParseDerivation( const QString& str );

} // namespace AutoUnits

#endif // AUTO_UNITS_DERIVATION_PARSER_H
