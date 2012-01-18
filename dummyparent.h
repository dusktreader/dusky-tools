#pragma once

#include "persist.h"
#include "dummy.h"

class DummyParent : public PersistXML
{
    Q_OBJECT

private:

    int _intVal;
    double _doubleVal;
    QString _stringVal;

    Dummy _child;

public:

    DummyParent( int intVal, double doubleVal, QString stringVal, int childIntVal, double childDoubleVal, QString childStringVal );

    bool operator ==( const DummyParent& other );

    int intVal() const { return _intVal; }
    double doubleVal() const { return _doubleVal; }
    QString stringVal() const { return _stringVal; }

    const Dummy& child() const { return _child; }

protected:

    virtual void readData( QDomElement& element );
    virtual void writeData( QDomElement& element );
};
