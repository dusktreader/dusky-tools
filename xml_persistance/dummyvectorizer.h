#ifndef DUMMYVECTORIZER_H
#define DUMMYVECTORIZER_H

#include <QVector>
#include <QVariant>

#include "persist.h"

class DummyVectorizer : public PersistXML
{
    Q_OBJECT

private:

    QVector<QVariant> _valueVector;



public:

    DummyVectorizer( int intVal, double doubleVal, QString stringVal );

    bool operator ==( const DummyVectorizer& other ) const;

    QVector<QVariant> valueVector() const { return _valueVector; }



protected:

    virtual void readData( QDomElement& element );
    virtual void writeData( QDomElement& element );

};

#endif // DUMMYVECTORIZER_H
