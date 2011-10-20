//==============================================================================
/// \file /home/tprince/dev/Venom/AutoUnits/Tools/UnitsGraph/Main.cpp
/// 
/// Contains the main function for the application.
///
//  Copyright 2011 AgLeader Technology, Inc.
//==============================================================================

#include <iostream>

#include <QTextStream>

#include "AutoUnits/DefinitionParser.h"
#include "AutoUnits/Dimension.h"
#include "AutoUnits/UnitSystem.h"
#include "AutoUnits/Unit.h"

using namespace AutoUnits;

struct Indent
{
    Indent( int depth ) : m_depth( depth ) { }
    int m_depth;
};

QString Conversion( const Unit& from, const Unit& to )
{
    return "TODO";
}

QTextStream& operator<<( QTextStream& out, const Indent& indent )
{
    for ( int i = 0; i < indent.m_depth; ++i )
    {
        out << "    ";
    }
    return out;
}

void Graph( QTextStream& out, const Unit& unit )
{
    const Dimension& dim( *unit.GetDimension() );

    out << Indent(2) << unit.Name() << ";\n";
    
    QList<const Unit*> other_units( dim.Units() );
    for ( int i = 0; i < other_units.count(); ++i )
    {
        if ( &unit == other_units[i] )
        {
            continue;
        }

        const Unit& other = *other_units[i];

        out << Indent(2) << unit.Name() << " -> " << other.Name() 
            << "[ label=\"" << Conversion( unit, other ) << "\" ];\n";
    }

}

void Graph( QTextStream& out, const UnitSystem& system )
{
    out << "digraph UnitSystem {\n";

    QList<const Unit*> units = system.Units();
    for ( int i = 0; i < units.count(); ++i )
    {
        out << Indent(1) << units[i]->Name() << ";\n";
    }

    QList<const Dimension*> dims = system.Dimensions();
    for ( int i = 0; i < dims.count(); ++i )
    {
        const Dimension *dim_p( dims[i] );
        out << Indent(1) << "subgraph cluster_" << dim_p->Name() << " {\n";

        QList<const Unit*> units = dim_p->Units();
        for ( int j = 0; j < units.count(); ++j )
        {
            Graph( out, *units[j] );
        }

        out << Indent(1) << "}\n";
    }
    out << "}\n";
}

int main()
{
    DefinitionParser parser( "../../UnitDefinitions.yaml" );

    QList<ParseError> errors( parser.Errors() );

    for ( int i = 0; i < errors.count(); ++i )
    {
        std::cerr << QString( errors[i] ).toStdString() << std::endl;
    }

    QTextStream out( stdout );

    std::auto_ptr<const UnitSystem> system_p( parser.TakeResult() );
    if ( system_p.get() )
    {
        Graph( out, *system_p );
    }
}

