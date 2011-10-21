#ifndef AUTO_UNITS_TEST_H
#define AUTO_UNITS_TEST_H

#include <QtTest/QtTest>

template<class T> 
class Test
{
public:
    Test() 
    {
        T * test_p = new T;

        QTest::qExec( test_p, QStringList() );
    }
};

#endif // AUTO_UNITS_TEST_H
