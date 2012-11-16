#ifndef DUMMYMAPPER_H
#define DUMMYMAPPER_H

#include <QMap>
#include <QString>
#include <QVariant>

#include "persist.h"

class DummyMapper : public PersistXML
{
    Q_OBJECT

private:

    QMap<QString, QVariant> _valueMap;



public:

    DummyMapper( int intVal, double doubleVal, QString stringVal );

    bool operator ==( const DummyMapper& other ) const;

    QMap<QString, QVariant> valueMap() const { return _valueMap; }



protected:

    virtual void readData( QDomElement& element );
    virtual void writeData( QDomElement& element );
};

#endif // DUMMYMAPPER_H
