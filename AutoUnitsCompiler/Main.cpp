#include <iostream>

#include <QString>

#include "AutoUnits/DefinitionParser.h"

int main()
{
    AutoUnits::DefinitionParser parser( "../UnitDefinitions.yaml" );

    QList<AutoUnits::ParseError> errors( parser.Errors() );
    
    for ( int i = 0; i < errors.count(); ++i )
    {
        std::cout << QString( errors[i] ).toStdString() << std::endl;
    }
}

