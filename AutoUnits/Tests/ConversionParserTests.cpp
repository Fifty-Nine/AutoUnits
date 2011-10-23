#include <QtTest/QtTest>

#include "Test.h"

#include "ConversionParser.h"
#include "Types/Conversion.h"

using namespace AutoUnits;

class ConversionParserTests : public QObject
{
    Q_OBJECT;

private:
    typedef std::auto_ptr<Conversion> ConversionPtr;

    void Verify( const ConversionPtr& conv_p, double value )
    {
        QCOMPARE( conv_p->Eval( 1.0 ), value );
        QCOMPARE( conv_p->Eval( 0.0 ), value );
        QCOMPARE( conv_p->Eval( value ), value );
    }
    void Verify( const ConversionPtr& lhs_p, const ConversionPtr& rhs_p )
    {
        QCOMPARE( lhs_p->Eval( 1.0 ), rhs_p->Eval( 1.0 ) );
        QCOMPARE( lhs_p->Eval( 0.0 ), rhs_p->Eval( 0.0 ) );
        QCOMPARE( lhs_p->Eval( 123.4 ), rhs_p->Eval( 123.4 ) );
    }

private slots:

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
        
        QCOMPARE( conv_p->Eval( 0.0 ), 3.0 );
        QCOMPARE( conv_p->Eval( 1.0 ), 4.0 );
        QCOMPARE( conv_p->Eval( 123.4 ), 126.4 );
    } 

    void AddCommutative()
    {
        ConversionPtr lhs_p( ParseConversion( "value + 3.0" ) );
        ConversionPtr rhs_p( ParseConversion( "3.0 + value" ) );

        Verify( lhs_p, rhs_p );
    }

    void AddAssociativity() 
    {
        ConversionPtr first_p( ParseConversion( "value + 3.0 + 7.0" ) );
        ConversionPtr second_p( ParseConversion( "(value + 3.0) + 7.0" ) );
        ConversionPtr third_p( ParseConversion( "value + (3.0 + 7.0)" ) );

        Verify( first_p, second_p );
        Verify( first_p, third_p );
    }

    void AddIdentity()
    {
        ConversionPtr lhs_p( ParseConversion( "value + 0.0" ) );
        ConversionPtr rhs_p( ParseConversion( "value" ) );

        Verify( lhs_p, rhs_p );
    }

    void BasicSubtract() 
    {
        ConversionPtr conv_p( ParseConversion( "value - 1.0" ) );

        QCOMPARE( conv_p->Eval( 0.0 ), -1.0 );
        QCOMPARE( conv_p->Eval( 1.0 ), 0.0 );
        QCOMPARE( conv_p->Eval( 123.4 ), 122.4 );
    }

    void SubtractIdentity()
    {
        ConversionPtr lhs_p( ParseConversion( "value - 0.0" ) );
        ConversionPtr rhs_p( ParseConversion( "value" ) );

        Verify( lhs_p, rhs_p );
    }
};

#include "ConversionParserTests.moc"

static Test<ConversionParserTests> s_test;
