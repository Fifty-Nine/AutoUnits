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
#include "Dimension.h"
#include "QtYaml.h"
#include "UnitSystem.h"

namespace
{

QString REDEFINED_DIM_NAME = "Redefinition of dimension \"%1\" on line %2.";
QString REDEFINED_DIM_ID = "Definition of dimension \"%1\" on line %2 conflicts"
                           " with definition of dimension \"%3\".";
QString REDEFINED_UNIT_NAME = "Redefinition of unit \"%1\" on line %2.";

}

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
DimensionId ParseDerivation( const QString& str )
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
    catch ( ParseError& err )
    {
        m_errors.append( err );
        m_result.reset();
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
void DefinitionParser::ParseDocument( const YAML::Node& document )
{
    ParseBaseDimensions( document["BaseDimensions"] );
    ParseDerivedDimensions( document["DerivedDimensions"] );
    ParseConvertedUnits( document["ConvertedUnits"] );
}

//==============================================================================
/// Parse the base dimension list from a document.
/// 
/// \param [in] dim_list The YAML list node for the dimensions.
/// 
void DefinitionParser::ParseBaseDimensions( const YAML::Node& dim_list )
{
    for ( YAML::Iterator it = dim_list.begin(); it != dim_list.end(); ++it )
    {
        ParseBaseDimension( *it );
    }
}

//==============================================================================
/// Parse a single base dimension definition.
/// 
/// \param [in] dim The YAML map node for the dimension.
/// 
void DefinitionParser::ParseBaseDimension( const YAML::Node& dim )
{
    QString name; 
    dim["name"] >> name;

    QString unit_name; 
    dim["unit"] >> unit_name;

    DimensionId id;
    id[unit_name] = 1;

    Dimension *dim_p;
    Unit *unit_p;

    if ( ( dim_p = m_result->GetDimension( name ) ) )
    {
        throw ParseError( dim.GetMark().line, 
            REDEFINED_DIM_NAME.arg( name ).arg( dim.GetMark().line ) );
    }

    if ( ( dim_p = m_result->GetDimension( id ) ) )
    {
        throw ParseError( dim.GetMark().line, REDEFINED_DIM_ID.
            arg( name ).arg( dim.GetMark().line ).arg( dim_p->Name() ) );
    }

    if ( ( unit_p = m_result->GetUnit( unit_name ) ) )
    {
        throw ParseError( dim.GetMark().line, REDEFINED_UNIT_NAME.
            arg( unit_name ).arg( dim.GetMark().line ) );
    }

    dim_p = m_result->NewDimension( name, id );

    unit_p = m_result->NewUnit( unit_name, dim_p );

    dim_p->SetBaseUnit( unit_p );
}

//==============================================================================
/// Parse the derived dimension list from a document.
/// 
/// \param [in] dim_list The YAML list node for the dimensions.
/// 
void DefinitionParser::ParseDerivedDimensions( const YAML::Node& dim_list )
{
    for ( YAML::Iterator it = dim_list.begin(); it != dim_list.end(); ++it )
    {
        ParseDerivedDimension( *it );
    }
}

//==============================================================================
/// Parse a single derived dimension definition.
/// 
/// \param [in] dim The YAML map node for the dimension.
/// 
void DefinitionParser::ParseDerivedDimension( const YAML::Node& dim )
{
    (void)dim;
}

//==============================================================================
/// Parse the converted units list from a document.
/// 
/// \param [in] unit_list The list of converted units.
/// 
void DefinitionParser::ParseConvertedUnits( const YAML::Node& unit_list )
{
    for ( YAML::Iterator it = unit_list.begin(); it != unit_list.end(); ++it )
    {
        ParseConvertedUnit( *it );
    }
}

//==============================================================================
/// Parse a single converted unit definition.
/// 
/// \param [in] unit The YAML map node for the unit.
/// 
void DefinitionParser::ParseConvertedUnit( const YAML::Node& unit )
{
    (void)unit;
}

//==============================================================================
/// Constructor.
/// 
/// \param [in] line The line number.
/// \param [in] description The description of the error.
/// 
ParseError::ParseError( int line, const QString& description ) : 
    m_line( line ), m_desc( description )
{
}

//==============================================================================
/// Convert the error to a QString.
/// 
/// \return A string representation of the error.
/// 
ParseError::operator QString() const
{
    static const QString FORMAT = "%1:%2";
    return FORMAT.arg( m_line ).arg( m_desc );
}

} // namespace AutoUnits

