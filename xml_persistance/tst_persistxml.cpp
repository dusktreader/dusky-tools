#include <QtCore/QString>
#include <QtTest/QtTest>
#include <QtCore/QCoreApplication>

#include "dummy.h"
#include "dummyparent.h"
#include "dummymapper.h"
#include "dummyvectorizer.h"

class PersistXMLTest : public QObject
{
    Q_OBJECT

public:
    PersistXMLTest();

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void readWrite_test();
    void readWriteParent_test();
    void readWriteMap_test();
    void readWriteVector_test();
};

PersistXMLTest::PersistXMLTest()
{
}

void PersistXMLTest::initTestCase()
{
}

void PersistXMLTest::cleanupTestCase()
{
}

void PersistXMLTest::readWrite_test()
{
    QFile f( "test.xml" );
    f.open( QFile::WriteOnly );
    Dummy a( 1, 2.0, "3" );
    a.save( f, "PersistTest" );
    f.close();

    QFile g( "test.xml" );
    g.open( QFile::ReadOnly );
    Dummy b( -1, -1.0, "-1" );
    b.load( g, "PersistTest"  );
    g.close();

    QVERIFY( a == b );
}

void PersistXMLTest::readWriteParent_test()
{
    QFile f( "test2.xml" );
    f.open( QFile::WriteOnly );
    DummyParent a( 1, 2.0, "3", 4, 5.0, "6" );
    a.save( f, "ParentPersistTest" );
    f.close();

    QFile g( "test2.xml" );
    g.open( QFile::ReadOnly );
    DummyParent b( -1, -1.0, "-1", -1, -1.0, "-1" );
    b.load( g, "ParentPersistTest" );
    g.close();

    QVERIFY( a == b );
}

void PersistXMLTest::readWriteMap_test()
{
    QFile f( "test3.xml" );
    f.open( QFile::WriteOnly );
    DummyMapper a( 1, 2.0, "3" );
    a.save( f, "MapPersistTest" );
    f.close();

    QFile g( "test3.xml" );
    g.open( QFile::ReadOnly );
    DummyMapper b( -1, -1.0, "-1" );
    b.load( g, "MapPersistTest" );
    g.close();

    QVERIFY( a == b );
}

void PersistXMLTest::readWriteVector_test()
{
    QFile f( "test4.xml" );
    f.open( QFile::WriteOnly );
    DummyVectorizer a( 1, 2.0, "3" );
    a.save( f, "VectorPersistTest" );
    f.close();

    QFile g( "test4.xml" );
    g.open( QFile::ReadOnly );
    DummyVectorizer b( -1, -1.0, "-1" );
    b.load( g, "VectorPersistTest" );
    g.close();

    QVERIFY( a == b );
}

QTEST_MAIN(PersistXMLTest)

#include "tst_persistxml.moc"
