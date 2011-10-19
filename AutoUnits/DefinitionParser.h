#ifndef AUTO_UNITS_DEFINITION_PARSER_H
#define AUTO_UNITS_DEFINITION_PARSER_H
//==============================================================================
/// \file AutoUnits/DefinitionParser.h
/// 
/// Header file for the unit definition parser.
///
//  Copyright 2011 AgLeader Technology, Inc.
//==============================================================================

#include <memory>

#include <QList>
#include <QString>

namespace YAML { class Node; }

namespace AutoUnits
{

class ParseError;
class UnitSystem;

//==============================================================================
/// A class for parsing unit system definition YAML files.
/// 
class DefinitionParser
{
public:
    DefinitionParser( const QString& path );
    ~DefinitionParser();

    std::auto_ptr<const UnitSystem> TakeResult();
    QList<ParseError> Errors() const;

private:
    void ParseFile( const QString& path );
    void ParseDocument( const YAML::Node& document );

    void ParseBaseDimensions( const YAML::Node& dim_list );
    void ParseBaseDimension( const YAML::Node& dim );

    void ParseDerivedDimensions( const YAML::Node& dim_list );
    void ParseDerivedDimension( const YAML::Node& dim );

    void ParseConvertedUnits( const YAML::Node& unit_list );
    void ParseConvertedUnit( const YAML::Node& unit );

    /// The errors that were encountered in the parse.
    QList<ParseError> m_errors;

    /// The result of the parse.
    std::auto_ptr<UnitSystem> m_result;

    /// Not implemented.
    DefinitionParser();
    /// Not implemented.
    DefinitionParser( const DefinitionParser& );
    /// Not implemented.
    DefinitionParser& operator=( const DefinitionParser& );
};

//==============================================================================
/// A class for reporting parse errors.
/// 
class ParseError
{
public:
    ParseError( int line, const QString& description );

    operator QString() const;

    /// The line number.
    int m_line;
    /// The error description.
    QString m_desc;
};

} // namespace AutoUnits

#endif // AUTO_UNITS_DEFINITION_PARSER_H
