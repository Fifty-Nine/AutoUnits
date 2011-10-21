#ifndef AUTO_UNITS_TEST_H
#define AUTO_UNITS_TEST_H

#include <QtTest/QtTest>
#include <memory>

template<class T> 
class Test
{
public:
    Test() 
    {
        std::auto_ptr<T> test_p( new T );

        QTest::qExec( test_p.get(), QStringList() );
    }
};

#endif // AUTO_UNITS_TEST_H
