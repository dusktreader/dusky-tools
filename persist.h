#pragma once

#include <QObject>
#include <QtXml>
#include <QSharedPointer>

#include "utilities.hpp"
#include "qlocalassert.h"
#include "localassert.h"
#include "convert.hpp"

/// This is used to check the format of the XML files.  If changes are made to this clas, this flag should change
#define FORMAT_VERSION "ADAM"

/// This is used as a prefix for map containers keys in the XML files
#define MAP_KEY_PREFIX "KEY."

/// This is used as a prefix for serial containers keys in the XML files
#define VECTOR_INDEX_PREFIX "INDEX."

/// Provides persistence functionality and an API for easily persisting data in XML files.
class PersistXML
{

private:

    /** Used to check the version of the XML files.  That way, if a new format is put into place, the persistance
     *  module will not load the data file but raise an exception instead.
     */
    static QString _formatVersionName;



public:

    /// Constructs this PersistXML object
    PersistXML() : QObject(NULL){}

    /// Destructs this PersistXML
    virtual ~PersistXML(){}

    /// Returns a unique alias for this Persistance object
    QString alias() const;

    /// Writes a PersistXML to a data stream
    void save(
            QIODevice& saveDevice,                ///< The I/O Device to which the object should be saved
            QString saveTitle="GenericPersistXML" ///< The title under which to save the PersistXML object
            );

    /// Reads a PersistXML object from a data stream
    void load(
            QIODevice& loadDevice,                ///< The I/O Device from which the object should be loaded
            QString loadTitle="GenericPersistXML" ///< The title under which the PersistXML object should be found
    );

    /// Reads an PersistXML object
    void read(
            QDomElement parent, ///< The QDomElement that should contain data for this object
            QString tag         ///< The tag under which the data for this element should be found
            );

    /// Writes an XMLPersistance object
    void write(
            QDomElement& parent, ///< The QDomElement into which the data for this object will be written
            QString tag   ///< The tag under which the data for this element should be written
            );

    /// Fetches a particular child element beneath the given parent node
    static QDomElement fetchChild(
            const QDomElement& parent, ///< The parent node under which to search for the particular child
            const QString& tag,        ///< The tag that identifies the child node for which to search
            const QString& nodeType="" ///< The expected type of the node.
            );

    /// Creates a child element beneath the given parent
    static QDomElement createChild(
            QDomElement& parent, ///< The parent node under which to create the child node
            QString tag,         ///< The identification tag under which to create the child node
            QString nodeType     ///< The type of the child node
            );



protected:

    // XML Persistance API

    /// Performs any necessary pre-read operations
    virtual void preRead(){}

    /// Reads the inherited object's data from the XML element
    virtual void readData( QDomElement& element ) = 0;

    /// Performs any necessary post-read operations
    virtual void postRead(){}

    /// Performs any necessary pre-write operations
    virtual void preWrite(){}

    /// Writes the inherited object's data into the XML element
    virtual void writeData( QDomElement& element ) = 0;

    /// Performs any necessary post-write operations
    virtual void postWrite(){}



    /// Reads a map of generic items from an XML document
    static QMap<QString, QVariant> readVariantMap(
            QDomElement parent, ///< The parent node under which to search for the particular child
            QString tag         ///< The tag that identifies the child node for which to search
            );


    /// Writes a map of generic items to an XML document
    static void writeVariantMap(
            QDomElement& parent,                   ///< The parent node under which to search for the particular child
            QString tag,                           ///< The tag that identifies the child node for which to search
            QMap<QString, QVariant> persistanceMap ///< The map to be persisted to the XML document
            );



    /// Reads a vectorof generic items from an XML document
    static QVector<QVariant> readVariantVector(
            QDomElement parent, ///< The parent node under which to search for the particular child
            QString tag         ///< The tag that identifies the child node for which to search
            );


    /// Writes a map of generic items to an XML document
    static void writeVariantVector(
            QDomElement& parent,                   ///< The parent node under which to search for the particular child
            QString tag,                           ///< The tag that identifies the child node for which to search
            QVector<QVariant> persistanceVector    ///< The map to be persisted to the XML document
            );


    /** @brief  Reads a vector of PersistXML type objects from an XML file
      * @return A vector of QSharedPointer objects referencing the template class instances
      * @note   T must be a derived type of PersistXML, and must have a valid default constructor.
      */
    template <class T>
    QVector< QSharedPointer<T> > readPersistVector(
            QDomElement parent, ///< The parent node under which to search for the particular child
            QString tag         ///< The tag that identifies the child node for which to search
            )
    {
        QDomElement child = fetchChild( parent, tag, "QVector<PersistXMLPtr>" );

        int count = child.attribute( "count" ).toInt( &ok );
        ASSERT_MSG( ok, "Couldn't extract vector size" );
        QVector< QSharedPointer<T> > persistVector = QVector< QSharedPointer<T> >( count );

        for( int i=0; i<count; i++ )
        {
            QString itemTag = QString( "%1%2" ).arg( VECTOR_INDEX_PREFIX ).arg( i );
            persistVector[i] = QSharedPointer<T>( new T() );
            persistVector[i]->read( child, itemTag );
        }

        return persistVector;
    }



    /** @brief  Writes a vector of PersistXML type objects from an XML file
      * @note   T must be a derived type of PersistXML.
      */
    template <class T>
    void writePersistVector(
            QDomElement parent,                        ///< The parent node under which to store the particular child
            QString tag,                               ///< The tag that identifies the child node to be stored
            QVector< QSharedPointer<T> > persistVector ///< The vector of Persistance items to write
            )
    {
        QDomElement child = createChild( parent, tag, "QVector<QVariant>" );
        child.setAttribute( "count", persistVector.size() );

        for( int i=0; i<persistVector.size(); i++ )
        {
            QString itemTag = QString( "%1%2" ).arg( VECTOR_INDEX_PREFIX ).arg( i );
            persistVector[i]->write( child, itemTag );
        }
        parent.appendChild( child );
    }
};
