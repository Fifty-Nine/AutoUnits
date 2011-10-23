#include <QtTest/QtTest>
#include <cmath>
#include <limits>

#include "Test.h"

#include "ConversionParser.h"
#include "Types/Conversion.h"

using namespace AutoUnits;

class ConversionParserTests : public QObject
{
    Q_OBJECT;

private:
    typedef std::auto_ptr<Conversion> ConversionPtr;

    bool Compare( double l, double r )
    {
        return 
            std::abs( l - r ) < ( 1.0e-10 );
    }

    bool Compare( const ConversionPtr& lhs_p, const ConversionPtr& rhs_p )
    {
        return Compare( lhs_p->Eval( 1e-13 ), rhs_p->Eval( 1e-13 ) ) &&
            Compare( lhs_p->Eval( 1.0 ), rhs_p->Eval( 1.0 ) ) &&
            Compare( lhs_p->Eval( 123.4 ), rhs_p->Eval( 123.4 ) );
    }

    bool Compare( const ConversionPtr& lhs_p, double rhs )
    {
        return Compare( lhs_p->Eval( 1e-13 ), rhs ) &&
            Compare( lhs_p->Eval( 1.0 ), rhs ) &&
            Compare( lhs_p->Eval( 123.4 ), rhs );
    }

    bool Compare( double lhs, const ConversionPtr& rhs_p )
    {
        return Compare( rhs_p, lhs );
    }

    bool Throws( const QString& str )
    {
        try
        {
            ParseConversion( str );
        } 
        catch (...)
        {
            return true;
        }
        return false;
    }

    void RunTest( const QString& first, const QString& second, 
        const QString& third, bool expected_equal )
    {
        ConversionPtr first_p( ParseConversion( first ) );
        ConversionPtr second_p( ParseConversion( second ) );
        
        bool eq = Compare( first_p, second_p );

        QVERIFY( eq == expected_equal );

        if ( !third.isEmpty() )
        {
            ConversionPtr third_p( ParseConversion( third ) );

            eq = Compare( first_p, third_p );

            QVERIFY( eq );
        }
    
    }
private slots:

    void Addition_data()
    {
        QTest::addColumn<QString>( "first" );
        QTest::addColumn<QString>( "second" );
        QTest::addColumn<QString>( "third" );
        QTest::addColumn<bool>( "expected_equal" );

        QTest::newRow( "commutative" ) 
            << "value + 3.0" 
            << "3.0 + value" 
            << "" << true;
        QTest::newRow( "associative" ) 
            << "3.0 + value + 7.0" 
            << "(3.0 + value) + 7.0" 
            << "3.0 + (value + 7.0)" 
            << true;
        QTest::newRow( "identity" ) 
            << "value + 0.0"
            << "value"
            << "" << true;
    }

    void Subtraction_data()
    {
        QTest::addColumn<QString>( "first" );
        QTest::addColumn<QString>( "second" );
        QTest::addColumn<QString>( "third" );
        QTest::addColumn<bool>( "expected_equal" );

        QTest::newRow( "commutative" ) 
            << "value - 3.0" 
            << "3.0 - value" 
            << "" << false;
        QTest::newRow( "associative" ) 
            << "3.0 - value - 7.0" 
            << "3.0 - (value - 7.0)" 
            << "(3.0 - value) - 7.0" 
            << false;
        QTest::newRow( "identity" ) 
            << "value - 0.0"
            << "value"
            << "" << true;
        QTest::newRow( "zero" )
            << "value - value"
            << "0.0"
            << "" << true;
    }
    
    void Multiplication_data()
    {
        QTest::addColumn<QString>( "first" );
        QTest::addColumn<QString>( "second" );
        QTest::addColumn<QString>( "third" );
        QTest::addColumn<bool>( "expected_equal" );

        QTest::newRow( "commutative" ) 
            << "value * 3.0" 
            << "3.0 * value" 
            << "" << true;
        QTest::newRow( "associative" ) 
            << "3.0 * value * 7.0" 
            << "3.0 * (value * 7.0)" 
            << "(3.0 * value) * 7.0" 
            << true;
        QTest::newRow( "identity" ) 
            << "value * 1.0"
            << "value"
            << "" << true;
        QTest::newRow( "zero" ) 
            << "value * 0.0"
            << "0.0"
            << "" << true;
    }

    void Division_data()
    {
        QTest::addColumn<QString>( "first" );
        QTest::addColumn<QString>( "second" );
        QTest::addColumn<QString>( "third" );
        QTest::addColumn<bool>( "expected_equal" );

        QTest::newRow( "commutative" ) 
            << "value / 3.0" 
            << "3.0 / value" 
            << "" << false;
        QTest::newRow( "associative" ) 
            << "3.0 / value / 7.0" 
            << "3.0 / (value / 7.0)" 
            << "(3.0 / value) / 7.0" 
            << false;
        QTest::newRow( "identity" ) 
            << "value / 1.0"
            << "value"
            << "" << true;
        QTest::newRow( "zero" ) 
            << "0.0 / value"
            << "0.0"
            << "" << true;
    }

    void Composites_data()
    {
        QTest::addColumn<QString>( "first" );
        QTest::addColumn<QString>( "second" );
        QTest::addColumn<QString>( "third" );
        QTest::addColumn<bool>( "expected_equal" );

        QTest::newRow( "mult_over_add_distributive" ) 
            << "value * 3.0 + 7.0 * value" 
            << "value * 10.0" 
            << "" << true;

        QTest::newRow( "multiplicative_inverse" )
            << "1.0 / (value + 1.0) * (value + 1.0)"
            << "1.0"
            << "" << true;

        QTest::newRow( "order_of_ops_mul_div" )
            << "value / 10.0 * 3.0"
            << "(value / 10.0) * 3.0"
            << "" << true;

        QTest::newRow( "order_of_ops_add_sub" ) 
            << "value - 10.0 + 3.0"
            << "(value - 10.0) + 3.0"
            << "" << true;

        QTest::newRow( "order_of_ops_add_mul" )
            << "value + 10.0 * 3.0"
            << "value + (10.0 * 3.0)"
            << "" << true;

        QTest::newRow( "order_of_ops_add_div" )
            << "value + 10.0 / 3.0"
            << "value + (10.0 / 3.0)"
            << "" << true;

        QTest::newRow( "order_of_ops_sub_mul" )
            << "value - 10.0 * 3.0"
            << "value - (10.0 * 3.0)"
            << "" << true;
        
        QTest::newRow( "order_of_ops_sub_div" )
            << "value - 10.0 / 3.0"
            << "value - (10.0 / 3.0)"
            << "" << true;
    }

    void Addition()
    {
        QFETCH(QString, first);
        QFETCH(QString, second);
        QFETCH(QString, third);
        QFETCH(bool, expected_equal);

        RunTest( first, second, third, expected_equal );
    }

    void Subtraction()
    {
        QFETCH(QString, first);
        QFETCH(QString, second);
        QFETCH(QString, third);
        QFETCH(bool, expected_equal);

        RunTest( first, second, third, expected_equal );
    }

    void Multiplication()
    {
        QFETCH(QString, first);
        QFETCH(QString, second);
        QFETCH(QString, third);
        QFETCH(bool, expected_equal);

        RunTest( first, second, third, expected_equal );
    }

    void Division()
    {
        QFETCH(QString, first);
        QFETCH(QString, second);
        QFETCH(QString, third);
        QFETCH(bool, expected_equal);

        RunTest( first, second, third, expected_equal );
    }

    void Composites()
    {
        QFETCH(QString, first);
        QFETCH(QString, second);
        QFETCH(QString, third);
        QFETCH(bool, expected_equal);

        RunTest( first, second, third, expected_equal );
    }

    void SimpleConstant()
    {
        ConversionPtr conv_p( ParseConversion( "123.4" ) );

        QCOMPARE( conv_p->Eval( 0.0 ), 123.4 );
        QCOMPARE( conv_p->Eval( 1.0 ), 123.4 );
        QCOMPARE( conv_p->Eval( 123.4 ), 123.4 );
    }

    void SimpleValue()
    {
        ConversionPtr conv_p( ParseConversion( "value" ) );

        QCOMPARE( conv_p->Eval( 0.0 ), 0.0 );
        QCOMPARE( conv_p->Eval( 1.0 ), 1.0 );
        QCOMPARE( conv_p->Eval( 123.4 ), 123.4 );
    }

    void BasicAddition()
    {
        ConversionPtr conv_p( ParseConversion( "value + 3.0" ) );
        
        QVERIFY( Compare( conv_p->Eval( 0.0 ), 3.0 ) );
        QVERIFY( Compare( conv_p->Eval( 1.0 ), 4.0 ) );
        QVERIFY( Compare( conv_p->Eval( 123.4 ), 126.4 ) );
    } 

    void BasicSubtraction() 
    {
        ConversionPtr conv_p( ParseConversion( "value - 1.0" ) );

        QCOMPARE( conv_p->Eval( 0.0 ), -1.0 );
        QCOMPARE( conv_p->Eval( 1.0 ), 0.0 );
        QCOMPARE( conv_p->Eval( 123.4 ), 122.4 );
    }

    void BasicMultiplication()
    {
        ConversionPtr conv_p( ParseConversion( "value * 3.0" ) );
        
        QVERIFY( Compare( conv_p->Eval( 0.0 ), 0.0 ) );
        QVERIFY( Compare( conv_p->Eval( 1.0 ), 3.0 ) );
        QVERIFY( Compare( conv_p->Eval( 123.4 ), 370.2 ) );
    } 

    void BasicDivision() 
    {
        ConversionPtr conv_p( ParseConversion( "value / 3.0" ) );

        QCOMPARE( conv_p->Eval( 0.0 ), 0.0 );
        QCOMPARE( conv_p->Eval( 3.0 ), 1.0 );
        QCOMPARE( conv_p->Eval( 123.3 ), 41.1 );
    }

    void ConstantFolding()
    {
        ConversionPtr lhs_p( ParseConversion( "10.0 * 3.0 - 12.0 / 4.0" ) );
        ConversionPtr rhs_p( ParseConversion( "27.0" ) );

        QVERIFY( Compare( lhs_p, rhs_p ) );
    }

    void DivisionByZero()
    {
        QVERIFY( Throws( "value / 0.0" ) );
    }

    void ConversionComposition()
    {
        ConversionPtr to_p( ParseConversion( "(value - 32) * 5.0 / 9.0" ) );
        ConversionPtr from_p( ParseConversion( "value * 9.0 / 5 + 32" ) );
        ConversionPtr expect_p( ParseConversion( "value" ) );

        ConversionPtr composed0_p( Compose( to_p, from_p ) );
        ConversionPtr composed1_p( Compose( from_p, to_p ) );

        QVERIFY( Compare( composed0_p, expect_p ) );
        QVERIFY( Compare( composed1_p, expect_p ) );
    }
};

#include "ConversionParserTests.moc"

static Test<ConversionParserTests> s_test;
