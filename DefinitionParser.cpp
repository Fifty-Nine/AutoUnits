//==============================================================================
/// \file AutoUnits/DefinitionParser.cpp
/// 
/// Source file for the unit definition parser.
///
//  Copyright 2011 AgLeader Technology, Inc.
//==============================================================================

#include <fstream>
#include <yaml-cpp/yaml.h>

#include <QFileInfo>

#include "DefinitionParser.h"
#include "UnitSystem.h"

namespace AutoUnits
{

namespace
{

//==============================================================================
/// Parse a derived dimension derivation string.
/// 
/// \param [in] str The derivation.
/// 
/// \return The dimension ID.
/// 
DimensionId ParseDerivation( const std::string& str )
{
    (void)str;
    return DimensionId();
}

} // namespace

//==============================================================================
/// Constructor.
/// 
/// \param [in] path The path to the file.
/// 
DefinitionParser::DefinitionParser( const QString& path )
{
    m_result = UnitSystem::Create();

    try
    {
        ParseFile( path );
    }
    catch ( YAML::Exception& ex )
    {
        m_errors.append( ParseError( ex.mark.line, ex.what() ) );
        m_result.reset();
    }
}

//==============================================================================
/// Destructor.
/// 
DefinitionParser::~DefinitionParser()
{
}

//==============================================================================
/// Take the result from the parser.
/// 
/// \return The result, or NULL if parsing failed or the result was already 
/// taken.
/// 
std::auto_ptr<const UnitSystem> DefinitionParser::TakeResult()
{
    return std::auto_ptr<const UnitSystem>( m_result.release() );
}

//==============================================================================
/// Get the list of errors encountered while parsing.
/// 
/// \return The list of errors.
/// 
QList<ParseError> DefinitionParser::Errors() const
{
    return m_errors;
}

//==============================================================================
/// Parse the file with the given path.
/// 
/// \param [in] path The path to the file.
/// 
/// 
void DefinitionParser::ParseFile( const QString& path )
{
    std::ifstream in( qPrintable( path ) );
    YAML::Parser parser( in );

    YAML::Node document;

    if ( !parser.GetNextDocument( document ) )
    {
        return;
    }

    ParseDocument( document );

    /// \todo Warn about unused documents?
}

//==============================================================================
/// Parse a YAML document.
/// 
/// \param [in] document The document to parse.
/// 
void DefinitionParser::ParseDocument( YAML::Node& document )
{
    (void)document;
}

//==============================================================================
/// Parse the base dimension list from a document.
/// 
/// \param [in] dim_list The YAML list node for the dimensions.
/// 
void DefinitionParser::ParseBaseDimensions( YAML::Node& dim_list )
{
    (void)dim_list;
}

//==============================================================================
/// Parse a single base dimension definition.
/// 
/// \param [in] dim The YAML map node for the dimension.
/// 
void DefinitionParser::ParseBaseDimension( YAML::Node& dim )
{
    (void)dim;
}

//==============================================================================
/// Parse the derived dimension list from a document.
/// 
/// \param [in] dim_list The YAML list node for the dimensions.
/// 
void DefinitionParser::ParseDerivedDimensions( YAML::Node& dim_list )
{
    (void)dim_list;
}

//==============================================================================
/// Parse a single derived dimension definition.
/// 
/// \param [in] dim The YAML map node for the dimension.
/// 
void DefinitionParser::ParseDerivedDimension( YAML::Node& dim )
{
    (void)dim;
}

//==============================================================================
/// Parse the converted units list from a document.
/// 
/// \param [in] unit_list The list of converted units.
/// 
void DefinitionParser::ParseConvertedUnits( YAML::Node& unit_list )
{
    (void)unit_list;
}

//==============================================================================
/// Parse a single converted unit definition.
/// 
/// \param [in] unit The YAML map node for the unit.
/// 
void DefinitionParser::ParseConvertedUnit( YAML::Node& unit )
{
    (void)unit;
}

} // namespace AutoUnits

