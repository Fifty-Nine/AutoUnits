#include <QtTest/QtTest>

#include "Test.h"

#include "DerivationParser.h"

using namespace AutoUnits;

class TestDerivationParser : public QObject
{
    Q_OBJECT;
private slots:
    void Scalar()
    {
        DimensionId id = ParseDerivation( "1" );

        QVERIFY( id.isEmpty() );
    }

    void Identifier()
    {
        DimensionId id0 = ParseDerivation( "Meter" );

        DimensionId id1;
        id1["Meter"] = 1;

        QVERIFY( id0 == id1 );
    }

    void BasicMultiplication()
    {
        DimensionId id0 = ParseDerivation( "M * S" );

        DimensionId id1;
        id1["M"] = 1;
        id1["S"] = 1;

        QVERIFY( id0 == id1 );
    }

    void MultiplicationIdentity()
    {
        DimensionId id0 = ParseDerivation( "M * 1" );
        DimensionId id1 = ParseDerivation( "M" );

        QVERIFY( id0 == id1 );
    }

    void MultiplicationCommutativity()
    {
        DimensionId id0 = ParseDerivation( "M * S" );
        DimensionId id1 = ParseDerivation( "S * M" );

        QVERIFY( id0 == id1 );
    }

    void MultiplicationAssociativity()
    {
        DimensionId id0 = ParseDerivation( "A * B * C" );
        DimensionId id1 = ParseDerivation( "(A * B) * C" );
        DimensionId id2 = ParseDerivation( "A * (B * C)" );

        QVERIFY( id0 == id1 );
        QVERIFY( id0 == id2 );
    }

    void MultiplicationZero()
    {
        DimensionId id0 = ParseDerivation( "A * 0" );
        DimensionId id1 = ParseDerivation( "0" );

        QVERIFY( id0 == id1 );
    }

    void BasicDivision()
    {
        DimensionId id0 = ParseDerivation( "Meter / Second" );

        DimensionId id1;
        id1["Meter"] = 1;
        id1["Second"] = -1;

        QVERIFY( id0 == id1 );
    }

    void DivisionIdentity()
    {
        DimensionId id0 = ParseDerivation( "M / 1" );
        DimensionId id1 = ParseDerivation( "M" );

        QVERIFY( id0 == id1 );
    }

    void DivisionInverse()
    {
        DimensionId id0 = ParseDerivation( "1 / M" );
        DimensionId id1;
        id1["M"] = -1;

        QVERIFY( id0 == id1 );
    }

    void DivisionCommutativity()
    {
        DimensionId id0 = ParseDerivation( "A / B" );
        DimensionId id1 = ParseDerivation( "B / A" );

        QVERIFY( id0 != id1 );
    }

    void DivisionAssociativity()
    {
        DimensionId id0 = ParseDerivation( "A / B / C" );
        DimensionId id1 = ParseDerivation( "(A / B) / C" );
        DimensionId id2 = ParseDerivation( "A / (B / C)" );
        DimensionId id3 = ParseDerivation( "A * C / B" );

        QVERIFY( id0 == id1 );
        QVERIFY( id0 != id2 );
        QVERIFY( id2 == id3 );
    }

    void DivisionZero()
    {
        DimensionId id0 = ParseDerivation( "0 / A" );
        DimensionId id1 = ParseDerivation( "0" );

        QVERIFY( id0 == id1 );
    }

    void DivisionByZero()
    {
        bool caught_exception = false;
        try
        {
            ParseDerivation( "A / 0" );
        }
        catch (...)
        {
            caught_exception = true;
        }
        QVERIFY( caught_exception );
    }

    void MultiplicativeInverse()
    {
        DimensionId id0 = ParseDerivation( "X * (1 / X)" );
        DimensionId id1 = ParseDerivation( "1" );

        QVERIFY( id0 == id1 );
    }

    void BasicExponentiation()
    {
        DimensionId id0 = ParseDerivation( "Meters^2" );
        DimensionId id1;
        id1["Meters"] = 2;

        QVERIFY( id0 == id1 );
    }

    void ExponentiationIdentity()
    {
        DimensionId id0 = ParseDerivation( "M^1" );
        DimensionId id1 = ParseDerivation( "M" );

        QVERIFY( id0 == id1 );
    }

    void ExponentiationZero()
    {
        DimensionId id0 = ParseDerivation( "A ^ 0" );
        DimensionId id1 = ParseDerivation( "1" );

        QVERIFY( id0 == id1 );
    }

    void ExponentiationIntLhs()
    {
        try
        {
            ParseDerivation( "1 ^ 1" );
            QFAIL( "Exception not thrown." );
        } catch (...) { }
    }

    void ExponentiationDimensionRhs()
    {
        try
        {
            ParseDerivation( "A ^ B" );
            QFAIL( "Exception not thrown." );
        } catch (...) { }
    }

    void OperatorPrecedence()
    {
        DimensionId id0 = ParseDerivation( "A / B * C" );
        DimensionId id1 = ParseDerivation( "(A / B) * C" );
        DimensionId id2 = ParseDerivation( "C * A / B" );
        DimensionId id3 = ParseDerivation( "(C * A) / B" );

        QVERIFY( id0 == id1 );
        QVERIFY( id0 == id2 );
        QVERIFY( id0 == id3 );
        
        DimensionId id4 = ParseDerivation( "A * B^2 / C^2" );
        DimensionId id5 = ParseDerivation( "A * (B^2) / (C^2)" );
        
        QVERIFY( id4 == id5 );
    }

    void NestedParens()
    {
        DimensionId id0 = ParseDerivation( "((A * B) / C) ^ 2" );
        DimensionId id1 = ParseDerivation( "(A * B / C) ^ 2" );

        QVERIFY( id0 == id1 );
    }

    void MissingRParen()
    {
        try
        {
            ParseDerivation( "(A * B" );
            QFAIL( "Exception not thrown." );
        } 
        catch (...) { }
    }
    void MissingLParen()
    {
        try
        {
            ParseDerivation( "A * B)" );
            QFAIL( "Exception not thrown." );
        } catch (...) { }
    }

    void PrecedingOp()
    {
        try
        {
            ParseDerivation( "* A" );
            QFAIL( "Exception not thrown." );
        } catch (...) { }
    }

    void TrailingOp()
    {
        try
        {
            ParseDerivation( "A *" );
            QFAIL( "Exception not thrown." );
        } catch (...) { }
    }

    void MissingOp()
    {
        try
        {
            ParseDerivation( "A B" );
            QFAIL( "Exception not thrown." );
        } catch (...) { }
    }

    void ExtraWhitespace()
    {
        DimensionId id0 = ParseDerivation( "\n\tA     *B   " );
        DimensionId id1 = ParseDerivation( "A*B" );

        QVERIFY( id0 == id1 );
    }
};

#include "DerivationParserTests.moc"

Test<TestDerivationParser> s_test;
