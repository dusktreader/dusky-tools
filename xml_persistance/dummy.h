#ifndef DUMMY_H
#define DUMMY_H

#include "persist.h"

class Dummy : public PersistXML
{
    Q_OBJECT

private:

    int _intVal;
    double _doubleVal;
    QString _stringVal;


public:

    Dummy( int intVal, double doubleVal, QString stringVal );

    bool operator ==( const Dummy& other ) const;

    int intVal() const { return _intVal; }
    double doubleVal() const { return _doubleVal; }
    QString stringVal() const { return _stringVal; }

protected:

    virtual void readData( QDomElement& element );
    virtual void writeData( QDomElement& element );
};

#endif // DUMMY_H
