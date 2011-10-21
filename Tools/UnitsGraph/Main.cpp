//==============================================================================
/// \file /home/tprince/dev/Venom/AutoUnits/Tools/UnitsGraph/Main.cpp
/// 
/// Contains the main function for the application.
///
//  Copyright 2011 AgLeader Technology, Inc.
//==============================================================================

#include <iostream>

#include <QTextStream>

#include "AutoUnits/Converter.h"
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

QTextStream& operator<<( QTextStream& out, const Indent& indent )
{
    for ( int i = 0; i < indent.m_depth; ++i )
    {
        out << "    ";
    }
    return out;
}

class Grapher
{
public:
    Grapher( const UnitSystem *system_p ) : 
        m_converter( system_p )
    {
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

            double c = m_converter.Convert( unit.Name(), other.Name(), 1.0 );

            out << Indent(2) << unit.Name() << " -> " << other.Name() 
                << "[ label=\"" << c << "\" ];\n";
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
    
    Converter m_converter;
};
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
        Grapher g( system_p.get() );
        g.Graph( out, *system_p );
    }
}

