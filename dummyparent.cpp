#include "dummyparent.h"

DummyParent::DummyParent( int intVal, double doubleVal, QString stringVal,
                          int childIntVal, double childDoubleVal, QString childStringVal ) :
    _intVal(intVal),
    _doubleVal(doubleVal),
    _stringVal(stringVal),
    _child(childIntVal, childDoubleVal, childStringVal)
{}

void DummyParent::readData( QDomElement& element )
{
    bool ok = true;

    _intVal = element.attribute( "intVal" ).toInt( &ok );
    ASSERT_MSG( ok, "Failed to read intVal" );

    _doubleVal = element.attribute( "doubleVal" ).toDouble( &ok );
    ASSERT_MSG( ok, "Failed to read doubleVal" );

    _stringVal = element.attribute( "stringVal" );

    _child.read( element, "child" );
}



void DummyParent::writeData( QDomElement& element )
{
    element.setAttribute( "intVal",  _intVal );
    element.setAttribute( "doubleVal",  _doubleVal );
    element.setAttribute( "stringVal",  _stringVal );

    _child.write( element, "child" );
}



bool DummyParent::operator ==( const DummyParent& other )
{
    return other._intVal == _intVal &&
           other._doubleVal == _doubleVal &&
           other._stringVal == _stringVal &&
           other.child() == child();
}
