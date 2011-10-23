#ifndef AUTO_UNITS_TEST_H
#define AUTO_UNITS_TEST_H

#include <QtTest/QtTest>
#include <iostream>
#include <memory>

#include "Util/Error.h"

template<class T> 
class Test
{
public:
    Test() 
    {
        std::auto_ptr<T> test_p( new T );

        try
        {
            QTest::qExec( test_p.get(), QStringList() );
        } 
        catch ( const AutoUnits::Util::ErrorInterface& err )
        {
            std::cerr << "Unexpected exception thrown: " 
                << qPrintable( (QString)err ) << std::endl;
        }
    }
};

#endif // AUTO_UNITS_TEST_H
