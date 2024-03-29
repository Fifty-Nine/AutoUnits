#ifndef AUTOUNITS_QT_YAML_H
#define AUTOUNITS_QT_YAML_H
//==============================================================================
/// \file AutoUnits/QtYaml.h
/// 
/// Functions to simplify using Yaml-cpp with Qt objects.
/// 
//==============================================================================

#include <QList>
#include <QHash>
#include <QMap>
#include <QString>
#include <yaml-cpp/yaml.h>

namespace YAML
{

//==============================================================================
/// Stream in operator for YAML nodes and QStrings.
/// 
/// \param [in] in The input parser.
/// \param [out] string The output string.
/// 
/// \return A reference to the parser.
/// 
inline const YAML::Node& operator>>( const YAML::Node& in, QString& string );

} // namespace YAML

const YAML::Node& YAML::operator>>( const Node& in, QString& string )
{
    std::string std_string;
    in >> std_string;

    string = QString::fromStdString( std_string );
    return in;
}

#endif // AUTOUNITS_QT_YAML_H
