#include "dummy.h"

Dummy::Dummy( int intVal, double doubleVal, QString stringVal ) :
    _intVal( intVal ),
    _doubleVal( doubleVal ),
    _stringVal( stringVal )
{}

void Dummy::readData( QDomElement& element )
{
    bool ok = true;

    _intVal = element.attribute( "intVal" ).toInt( &ok );
    ASSERT_MSG( ok, "Failed to read intVal" );

    _doubleVal = element.attribute( "doubleVal" ).toDouble( &ok );
    ASSERT_MSG( ok, "Failed to read doubleVal" );

    _stringVal = element.attribute( "stringVal" );
}



void Dummy::writeData( QDomElement& element )
{
    element.setAttribute( "intVal",  _intVal );
    element.setAttribute( "doubleVal",  _doubleVal );
    element.setAttribute( "stringVal",  _stringVal );
}



bool Dummy::operator ==( const Dummy& other ) const
{
    return other._intVal == _intVal &&
           other._doubleVal == _doubleVal &&
           other._stringVal == _stringVal;
}
