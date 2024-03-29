#ifndef AUTO_UNITS_DEFINITION_PARSER_H
#define AUTO_UNITS_DEFINITION_PARSER_H
//==============================================================================
/// \file AutoUnits/DefinitionParser.h
/// 
/// Header file for the unit definition parser.
///
//==============================================================================

#include <memory>

#include <QList>
#include <QString>

#include "Types/DimensionId.h"
#include "Util/Error.h"

namespace YAML 
{ 
    class Node; 
    class Mark;
}

namespace AutoUnits
{

class Dimension;
class ParseError;
class Unit;
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
    void ParseFile();
    void ParseDocument( const YAML::Node& document );

    void ParseBaseDimensions( const YAML::Node& dim_list );
    void ParseBaseDimension( const YAML::Node& dim );

    void ParseDerivedDimensions( const YAML::Node& dim_list );
    void ParseDerivedDimension( const YAML::Node& dim );

    void ParseConvertedUnits( const YAML::Node& unit_list );
    void ParseConvertedUnit( const YAML::Node& unit );

    void ParseConversions( const YAML::Node& node, Unit *unit_p );

    Dimension *DefineDimension( const YAML::Mark& mark, const QString& dim_name,
        const DimensionId& id, const QString& name );
    Unit *DefineUnit( 
        const YAML::Mark& mark, const QString& name, Dimension *dim_p );

    /// The errors that were encountered in the parse.
    QList<ParseError> m_errors;

    /// The result of the parse.
    std::auto_ptr<UnitSystem> m_result;

    /// The name of the file being parsed.
    QString m_file;

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
class ParseError : public Util::ErrorInterface
{
public:
    /// The types of errors.
    /// \todo Replace with separate classes for each type.
    enum ErrorType
    {
        Error,
        Warning
    };

    ParseError( const QString& file, int line, const QString& description, 
        ErrorType error_type = Error );

    virtual operator QString() const;
    virtual void Throw();

    /// The file path.
    QString m_file;
    /// The line number.
    int m_line;
    /// The error description.
    QString m_desc;
    /// The error type.
    ErrorType m_error_type;
};

} // namespace AutoUnits

#endif // AUTO_UNITS_DEFINITION_PARSER_H
