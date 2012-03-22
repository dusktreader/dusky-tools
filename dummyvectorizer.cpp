#include "dummyvectorizer.h"

DummyVectorizer::DummyVectorizer( int intVal, double doubleVal, QString stringVal )
{
    _valueVector =  QVector<QVariant>( 3 );
    _valueVector[0] = intVal;
    _valueVector[1] = doubleVal;
    _valueVector[2] = stringVal;
}

void DummyVectorizer::readData( QDomElement& element )
{
    _valueVector = PersistXML::readVariantVector( element, "valueVector" );
}

void DummyVectorizer::writeData( QDomElement &element )
{
    PersistXML::writeVariantVector( element, "valueVector", _valueVector );
}

bool DummyVectorizer::operator ==( const DummyVectorizer& other ) const
{
    return other.valueVector() == valueVector();
}
