#include "dummymapper.h"

DummyMapper::DummyMapper( int intVal, double doubleVal, QString stringVal )
{
    _valueMap["intVal"] = intVal;
    _valueMap["doubleVal"] = doubleVal;
    _valueMap["stringVal"] = stringVal;
}

void DummyMapper::readData( QDomElement &element )
{
    _valueMap = PersistXML::readPersistanceMap( element, "valueMap" );
}

void DummyMapper::writeData( QDomElement &element )
{
    PersistXML::writePersistanceMap( element, "valueMap", _valueMap );
}

bool DummyMapper::operator ==( const DummyMapper& other ) const
{
    return other.valueMap() == valueMap();
}
