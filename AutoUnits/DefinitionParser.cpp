//==============================================================================
/// \file AutoUnits/DefinitionParser.cpp
/// 
/// Source file for the unit definition parser.
///
//  Copyright 2011 AgLeader Technology, Inc.
//==============================================================================

#include <QDebug>

#include <fstream>
#include <yaml-cpp/yaml.h>

#include <QFileInfo>
#include <QStack>
#include <QStringList>

#include "ConversionParser.h"
#include "DefinitionParser.h"
#include "DerivationParser.h"
#include "Dimension.h"
#include "QtYaml.h"
#include "Unit.h"
#include "UnitSystem.h"

namespace
{

QString ERROR_FORMAT = "%1:%2: error: %3";
QString WARNING_FORMAT = "%1:%2: warning: %3";
QString REDEFINED_DIM_NAME = "Redefinition of dimension \"%1\" on line %2.";
QString REDEFINED_DIM_ID = "Definition of dimension \"%1\" on line %2 conflicts"
                           " with definition of dimension \"%3\".";
QString UNDEFINED_DIM_NAME = "Unknown dimension \"%1\" near line %2.";
QString REDEFINED_UNIT_NAME = "Redefinition of unit \"%1\" on line %2.";

}

namespace AutoUnits
{

//==============================================================================
/// Constructor.
/// 
/// \param [in] path The path to the file.
/// 
DefinitionParser::DefinitionParser( const QString& path ) : 
    m_file( path )
{
    m_result = UnitSystem::Create();

    try
    {
        ParseFile();
    }
    catch ( ParseError& err )
    {
        m_errors.append( err );
        m_result.reset();
    }
    catch ( YAML::Exception& ex )
    {
        m_errors.append( ParseError( m_file, ex.mark.line, ex.what() ) );
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
void DefinitionParser::ParseFile()
{
    std::ifstream in( qPrintable( m_file ) );
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

    (void)DefineDimension( dim.GetMark(), name, id, unit_name );
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
    QString name;
    dim["name"] >> name;
    QString unit_name;
    dim["unit"] >> unit_name;
    QString derivation;
    dim["derivation"] >> derivation;

    DimensionId id = ParseDerivation( derivation );

    (void)DefineDimension( dim.GetMark(), name, id, unit_name );
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
    QString name;
    unit["name"] >> name;
    QString base_unit_name;

    const YAML::Node& dim_node( unit["dimension"] );
    QString dim_name;
    dim_node >> dim_name;


    Dimension *dim_p = m_result->GetDimension( dim_name );

    if ( !dim_p )
    {
        const YAML::Mark& mark( dim_node.GetMark() );
        throw ParseError( m_file, mark.line,
            UNDEFINED_DIM_NAME.arg( dim_name ).arg( mark.line ) );
    }

    Unit *unit_p = DefineUnit( unit.GetMark(), name, dim_p );
    ParseConversions( unit["conversion"], unit_p );
}

//==============================================================================
/// Parse the conversion expression and store it in the unit.
/// 
/// \param [in] node The node with the expression.
/// \param [in] unit_p The unit to set up/
/// 
void DefinitionParser::ParseConversions( const YAML::Node& node, Unit *unit_p )
{
    if ( node.Type() == YAML::NodeType::Scalar )
    {
        double value;
        node >> value;

        unit_p->SetToBase( Conversion::ScaleFactor( value ) );
        unit_p->SetFromBase( Conversion::ScaleFactor( 1.0 / value ) );
    }
    else
    {
        QString to_base;
        QString from_base;
        node[0] >> to_base;
        node[1] >> from_base;

        unit_p->SetToBase( ParseConversion( to_base ) );
        unit_p->SetFromBase( ParseConversion( from_base ) );
    }
}

//==============================================================================
/// Define a new dimension with the given parameters, throwing appropriate
/// errors when encountered.
/// 
Dimension *DefinitionParser::DefineDimension( const YAML::Mark& mark, 
    const QString& dim_name, const DimensionId& dim_id, 
    const QString& unit_name )
{
    Dimension *dim_p;
    Unit *unit_p;

    if ( ( dim_p = m_result->GetDimension( dim_name ) ) )
    {
        throw ParseError( m_file, mark.line, 
            REDEFINED_DIM_NAME.arg( dim_name ).arg( mark.line ) );
    }

    if ( ( dim_p = m_result->GetDimension( dim_id ) ) )
    {
        throw ParseError( m_file, mark.line, REDEFINED_DIM_ID.
            arg( dim_name ).arg( mark.line ).arg( dim_p->Name() ) );
    }

    dim_p = m_result->NewDimension( dim_name, dim_id );

    unit_p = DefineUnit( mark, unit_name, dim_p );

    dim_p->SetBaseUnit( unit_p );

    return dim_p;
}

//==============================================================================
/// Define a new unit with the given parameters, throwing appropriate
/// errors when encountered.
/// 
Unit *DefinitionParser::DefineUnit( const YAML::Mark& mark, 
    const QString& name, Dimension *dim_p )
{
    Unit *unit_p = m_result->GetUnit( name );

    if ( unit_p )
    {
        throw ParseError( m_file, mark.line, REDEFINED_UNIT_NAME.
            arg( name ).arg( mark.line ) );
    }

    unit_p = m_result->NewUnit( name, dim_p );

    return unit_p;
}

//==============================================================================
/// Constructor.
/// 
/// \param [in] file The file name.
/// \param [in] line The line number.
/// \param [in] description The description of the error.
/// \param [in] type The error type.
/// 
ParseError::ParseError( const QString& file, int line, 
    const QString& description, ErrorType type ) : 
    m_file( file ), m_line( line ), m_desc( description ), m_error_type( type )
{
}

//==============================================================================
/// Convert the error to a QString.
/// 
/// \return A string representation of the error.
/// 
ParseError::operator QString() const
{
    QString format = ( m_error_type == Error ) ? ERROR_FORMAT : WARNING_FORMAT;
    return ERROR_FORMAT.arg( m_file ).arg( m_line ).arg( m_desc );
}

//==============================================================================
/// [Re]throw the error.
///
void ParseError::Throw() 
{
    throw *this;
}

} // namespace AutoUnits

