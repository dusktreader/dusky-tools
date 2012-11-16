#pragma once

#include <fstream>

#include <QObject>
#include <QtXml>

#include "qtools.h"

/// This is used to check the format of the XML files.  If changes are made to this clas, this flag should change
#define FORMAT_VERSION "DYSPROSIUM"

/** @class   XMLPersistance
  * @author  Tucker Beck
  * @date    03/2011
  * @brief   Provides persistence functionality and an API for easily persisting data in XML files.
  */
class XMLPersistance
{
public:

    /** Destructs the XMLPersistance object. */
    virtual ~XMLPersistance(){}

    /** Saves an XMLPersistance object to a persistance file
      * @param  fileName     - The name of the persistance file in which to save the data
      * @param  rootNodeName - The name of the root node in the persistance file
      */
    void save( const QString& fileName, const QString& rootNodeName="XMLPersistance" )
    {
        QFileInfo saveFileInfo( fileName );
        QString path = saveFileInfo.absoluteFilePath();

        ASSERT_QMSG( !saveFileInfo.exists() || ( saveFileInfo.isWritable() ),
                     QString( "Can't write to save file: %1" ).arg(path) );

        QDomDocument xmlDoc( "XMLPersistance" );
        QDomElement root = xmlDoc.createElement( "root" );
        xmlDoc.appendChild( root );
        root.setAttribute( "FormatVersion", FORMAT_VERSION );
        write( root, rootNodeName );

        /// @todo put permission checks here?
        QFile saveFile( fileName );
        ASSERT_QMSG( saveFile.open( QIODevice::WriteOnly ),
                                    QString( "Couldn't open save file: %1" ).arg(path) );
        QTextStream saveOut( &saveFile );
        xmlDoc.save( saveOut, 4 );
        saveFile.close();

    }

    /** Loads an XMLPersistance object from a provided file name
      * @param  fileName     - The name of the persistance file from which to load the data
      * @param  rootNodeName - The name of the root node in the persistance file
      */
    void load( const QString& fileName, const QString& rootNodeName="XMLPersistance" )
    {
        QFileInfo loadFileInfo( fileName );
        ASSERT_MSG( loadFileInfo.suffix().toLower() == "xml", "Erronious extension" );

        QString path = loadFileInfo.absoluteFilePath();

        ASSERT_QMSG( loadFileInfo.exists(), QString( "Save file doesn't exist: %1" ).arg(path) );
        ASSERT_QMSG( loadFileInfo.isReadable(), QString( "Can't read save file: %1" ).arg(path) );

        QFile loadFile( path );
        loadFile.open( QIODevice::ReadOnly );

        ASSERT_QMSG( loadFile.isOpen(), QString( "Couldn't open save file: %1" ).arg(path) );

        QDomDocument xmlDoc( "XMLPersistance" );
        ASSERT_MSG( xmlDoc.setContent( &loadFile ), "Couldn't set content for xml document" );

        loadFile.close();

        QDomElement root = xmlDoc.firstChildElement( "root" );
        ASSERT_MSG( !root.isNull(), "Couldn't find root node" );
        ASSERT_MSG( root.attribute( "FormatVersion" ) == FORMAT_VERSION, "Outdated or invalid format" );
        read( root, rootNodeName );
    }

    /** Loads a XMLPersistance object
      * @param  parent - The QDomElement that should contain data for this object
      * @param  todo   - Use a try/catch block at this level to describe error locale
      */
    virtual void read( const QDomElement& parent, const QString& tag )
    {
        preRead();
        QDomElement child = fetchChild( parent, tag, alias() );
        if( child.isNull() )
            return;
        readData( child );
        postRead();
    }

    /** Saves an XMLPersistance object
      * @param  parent - The QDomElement that should contain data for this object
      */
    virtual void write( QDomElement& parent, const QString& tag )
    {
        preWrite();
        QDomElement child = createChild( parent, tag, alias() );
        writeData( child );
        parent.appendChild( child );
        postWrite();
    }

protected:

    /** Returns a class specific name to be used with XML persistance
      * @todo  Figure out a way to do this with some sort of introspection
      */
    virtual QString alias() = 0;

    /** Creates a child element beneath the given parent
      * @param  parent   - The parent node under which to create the child
      * @param  tag      - The tag that will identify the child node
      * @param  nodeType - A String description of the child's node type
      */
    static QDomElement createChild( QDomElement& parent, const QString& tag, const QString& nodeType )
    {
        QDomElement child = parent.firstChildElement( tag );
        if( !child.isNull() )
            parent.removeChild( child );
        child = parent.ownerDocument().createElement( tag );
        child.setAttribute( "NodeType", nodeType );
        parent.appendChild( child );
        return child;
    }

    /** Fetches a child element beneath the given parent
      * @param  parent   - The parent node under which to find the child
      * @param  tag      - The tag that identifies the child node to find
      * @param  nodeType - A String description of the child's node type.  This must match in order to fetch the child
      */
    static QDomElement fetchChild( const QDomElement& parent, const QString& tag, const QString& nodeType )
    {
        QDomElement element = parent.firstChildElement( tag );
        if( nodeType != "" && element.attribute( "NodeType" ) != nodeType )
            element.clear();
        return element;
    }

    /** Writes a vector of doubles into a xml document
      * @param  values  - The vector of double values to write
      * @param  label   - The text label with which to tag the xml entries
      * @param  element - The xml doucment node in which to write the vector
      */
    static void writeDoubleVector( const QVector<double>& values, const QString& tag, QDomElement& parent )
    {
        QDomElement child = createChild( parent, tag, "DoubleVector" );
        child.setAttribute( "count", values.size() );
        for( int i=0; i<values.size(); i++ )
        {
            QString tag = QString( "item--%1" ).arg( i );
            QDomElement childItem = child.ownerDocument().createElement( tag );
            childItem.setAttribute( "value", values[i] );
            child.appendChild( childItem );
        }
        parent.appendChild( child );
    }

    /** Reads a vector of doubles from an xml doucument
      * @param  values  - The vector of double values to read from the xml file
      * @param  label   - The text label with which to identify the matching xml entries
      * @param  element - The xml document node from which to read the vector
      */
    void readDoubleVector( QVector<double>& values, const QString& tag, QDomElement& parent )
    {
        QDomElement child = fetchChild( parent, tag, "DoubleVector" );
        ASSERT_MSG( !child.isNull(), QString( "Couldn't find matching node: %1" ).arg(tag).toStdString() );

        bool ok = true;
        int count = child.attribute( "count" ).toInt( &ok ); ASSERT(ok);
        values = QVector<double>( count, 0.0 );
        for( int i=0; i<count; i++ )
        {
            QString childItemTag = QString( "item--%1" ).arg(i);
            QDomElement childItem = child.firstChildElement( childItemTag );
            if( childItem.isNull() )
                values[i] = 0.0;
            else
                values[i] = childItem.attribute( "value" ).toDouble( &ok ); ASSERT(ok);
        }
    }

    /** Writes a map of doubles into a xml document
      * @param  dblMap - The map of double values to write
      * @param  tag    - The text label with which to tag the xml entries
      * @param  parent - The xml doucment node in which to write the map
      */
    void writeDoubleMap( const QMap<QString,double>& dblMap, const QString& tag, QDomElement& parent )
    {
        QDomElement child = createChild( parent, tag, "DoubleMap" );

        foreach( const QString& key, dblMap.keys() )
        {
            QString childItemTag = QString( "key--%1" ).arg(key);
            QDomElement childItem = child.ownerDocument().createElement( childItemTag );
            childItem.setAttribute( "value", dblMap[key] );
            child.appendChild( childItem );
        }
        parent.appendChild( child );
    }

    /** Reads a map of doubles from an xml doucument
      * @param  dblMap - The map of double values to read from the xml file
      * @param  tag    - The text label with which to identify the matching xml entries
      * @param  parent - The xml document node from which to read the map
      */
    void readDoubleMap( QMap<QString,double>& dblMap, const QString& tag, const QDomElement& parent )
    {
        QDomElement child = fetchChild( parent, tag, "DoubleMap" );
        ASSERT_MSG( !child.isNull(), QString( "Couldn't find matching node: %1" ).arg(tag).toStdString() );

        bool ok = true;
        dblMap.clear();

        QDomNodeList childItems = child.childNodes();
        for( int i=0; i<childItems.count(); i++ )
        {
            QDomElement childItem = childItems.item( i ).toElement();
            QString childItemTag = childItem.tagName();
            if( !childItemTag.startsWith( "key--" ) )
                continue;
            QString key   = childItemTag.remove( "key--" );
            double  value = childItem.attribute( "value" ).toDouble( &ok );  ASSERT( ok );
            dblMap[key] = value;
        }
    }

    /** Writes a map of bools into a xml document
      * @param  boolMap - The map of boolean values to write
      * @param  tag     - The text label with which to tag the xml entries
      * @param  parent  - The xml doucment node in which to write the map
      */
    void writeBoolMap( const QMap<QString,bool>& boolMap, const QString& tag, QDomElement& parent )
    {
        QDomElement child = createChild( parent, tag, "BooleanMap" );

        foreach( const QString& key, boolMap.keys() )
        {
            QString childItemTag = QString( "key--%1" ).arg(key);
            QDomElement childItem = child.ownerDocument().createElement( childItemTag );
            childItem.setAttribute( "value", boolMap[key] ? "True" : "False" );
            child.appendChild( childItem );
        }
        parent.appendChild( child );
    }

    /** Reads a map of bools from an xml doucument
      * @param  boolMap - The map of boolean values to read from the xml file
      * @param  tag     - The text label with which to identify the matching xml entries
      * @param  parent  - The xml document node from which to read the map
      */
    void readBoolMap( QMap<QString,bool>& boolMap, const QString& tag, const QDomElement& parent )
    {
        QDomElement child = fetchChild( parent, tag, "BooleanMap" );
        ASSERT_MSG( !child.isNull(), QString( "Couldn't find matching node: %1" ).arg(tag).toStdString() );

        boolMap.clear();

        QDomNodeList childItems = child.childNodes();
        for( int i=0; i<childItems.count(); i++ )
        {
            QDomElement childItem = childItems.item( i ).toElement();
            QString childItemTag = childItem.tagName();
            if( !childItemTag.startsWith( "key--" ) )
                continue;
            QString key   = childItemTag.remove( "key--" );
            boolMap[key] = childItem.attribute( "value" ) == "True";
        }
    }

    /** Writes a map of strings into a xml document
      * @param  stringMap - The map of string values to write
      * @param  tag       - The text label with which to tag the xml entries
      * @param  parent    - The xml doucment node in which to write the map
      */
    void writeStringMap( const QMap<QString,QString>& stringMap, const QString& tag, QDomElement& parent )
    {
        QDomElement child = createChild( parent, tag, "StringMap" );

        foreach( const QString& key, stringMap.keys() )
        {
            QString childItemTag = QString( "key--%1" ).arg(key);
            QDomElement childItem = child.ownerDocument().createElement( childItemTag );
            childItem.setAttribute( "value", stringMap[key] );
            child.appendChild( childItem );
        }
        parent.appendChild( child );
    }

    /** Reads a map of strings from an xml doucument
      * @param  stringMap - The map of string values to read from the xml file
      * @param  tag       - The text label with which to identify the matching xml entries
      * @param  parent    - The xml document node from which to read the map
      */
    void readStringMap( QMap<QString,QString>& stringMap, const QString& tag, const QDomElement& parent )
    {
        QDomElement child = fetchChild( parent, tag, "StringMap" );
        ASSERT_MSG( !child.isNull(), QString( "Couldn't find matching node: %1" ).arg(tag).toStdString() );

        stringMap.clear();

        QDomNodeList childItems = child.childNodes();
        for( int i=0; i<childItems.count(); i++ )
        {
            QDomElement childItem = childItems.item( i ).toElement();
            QString childItemTag = childItem.tagName();
            if( !childItemTag.startsWith( "key--" ) )
                continue;
            QString key   = childItemTag.remove( "key--" );
            stringMap[key] = childItem.attribute( "value" );
        }
    }

    /** Writes a QStringList to the xml document
      * @param  entries - The strings to write
      * @param  tag     - The text label with which to identify the matching xml entries
      * @param  parent  - The xml document node in which to write the StringList
      */
    void writeStringList( const QStringList& entries, const QString& tag, QDomElement& parent )
    {
        QDomElement child = createChild( parent, tag, "StringList" );
        child.setAttribute( "count", entries.size() );

        foreach( const QString& entry, entries )
        {
            QString childItemTag = QString( "entry--%1" ).arg( entry );
            QDomElement childItem = child .ownerDocument().createElement( childItemTag );
            child.appendChild( childItem );
        }
        parent.appendChild( child  );
    }

    /** Reads a QStringList from the xml document
      * @param  entries - The strings to read
      * @param  tag     - The text label with which to identify the matching xml entries
      * @param  parent  - The xml document node from which to read the StringList
      */
    void readStringList( QStringList& entries, const QString& tag, QDomElement& parent )
    {
        QDomElement child = fetchChild( parent, tag, "StringList" );
        ASSERT_MSG( !child.isNull(), QString( "Couldn't find matching node: %1" ).arg(tag).toStdString() );

        entries.clear();
        QDomNodeList childItems = child.childNodes();
        for( int i=0; i<childItems.count(); i++ )
        {
            QDomElement childItem = childItems.item( i ).toElement();
            QString childItemTag = childItem.tagName();
            if( !childItemTag.startsWith( "entry--" ) )
                continue;
            QString entry = childItemTag.remove( "entry--" );
            entries.append( entry );
        }
    }

    /** Writes a QMap of XMLPersistance items to the xml document
      * @param  entries - The items to write
      * @param  tag     - The text label with which to identify the matching xml entries
      * @param  parent  - The xml document node in which to write the items
      */
    template <class T>
    void writeChildMap( QMap<QString,T>& childMap, const QString& tag, QDomElement& parent )
    {
        QDomElement child = createChild( parent, tag, "ChildMap" );
        foreach( const QString& key, childMap.keys() )
        {
            QString childItemTag = QString( "key--%1" ).arg( key );
            childMap[key].write( child, childItemTag );
        }
        parent.appendChild( child );
    }

    /** Reads a QMap of XMLPersistance items from the xml document
    * @param  entries - The item to read
    * @param  tag     - The text label with which to identify the matching xml entries
    * @param  parent  - The xml document node from which to read the items
    * @todo   Figure out someting better than the clear parameter
    */
    template <class T>
    void readChildMap( QMap<QString,T>& childMap, const QString& tag, const QDomElement& parent, bool clearMap=true )
    {
        QDomElement child = fetchChild( parent, tag, "ChildMap" );
        ASSERT_MSG( !child.isNull(), QString( "Couldn't find matching node: %1" ).arg(tag).toStdString() );

        if( clearMap )
            childMap.clear();

        QDomNodeList childItems = child.childNodes();
        for( int i=0; i<childItems.size(); i++ )
        {
            QDomElement childItem = childItems.item(i).toElement();
            QString childItemTag = childItem.tagName();
            if( !childItemTag.startsWith( "key--" ) )
                continue;
            QString key = childItemTag;
            key.remove( "key--" );
            childMap[key].read( child, childItemTag );
        }
    }

    /** Writes a QVector of XMLPersistance items to the xml document
      * @param  childVector - The vector items to write
      * @param  tag         - The text label with which to identify the matching xml entries
      * @param  parent      - The xml document node in which to write the items
      */
    template <class T>
    void writeChildVector( QVector<T>& childVector, const QString& tag, QDomElement& parent )
    {
        QDomElement child = createChild( parent, tag, "ChildVector" );
        child.setAttribute( "count", childVector.size() );
        for( int i=0; i<childVector.size(); i++ )
        {
            QString childItemTag = QString( "item--%1" ).arg( i );
            childVector[i].write( child, childItemTag );
        }
        parent.appendChild( child );
    }

    /** Reads a QVector of XMLPersistance items to the xml document
      * @param  childVector - The vector items to read
      * @param  tag         - The text label with which to identify the matching xml entries
      * @param  parent      - The xml document node from which to read the items
      */
    template <class T>
    void readChildVector( QVector<T>& childVector, const QString& tag, const QDomElement& parent )
    {
        QDomElement child = fetchChild( parent, tag, "ChildVector" );
        ASSERT_MSG( !child.isNull(), QString( "Couldn't find matching node: %1" ).arg(tag).toStdString() );

        bool ok = true;
        int count = child.attribute( "count" ).toInt( &ok ); ASSERT(ok);
        childVector.clear();
        childVector = QVector<T>( count );
        QDomNodeList childItems = child.childNodes();
        for( int i=0; i<count; i++ )
        {
            QDomElement childItem = childItems.item(i).toElement();
            QString childItemTag = childItem.tagName();
            if( !childItemTag.startsWith( "item--" ) )
                continue;
            T childObj;
            childObj.read( child, childItemTag );
            int index = childItemTag.remove( "item-" ).toInt( &ok ); ASSERT( ok );
            childVector[index] = childObj;
        }
    }

    /** Provides the an API for committing pre-read operations */
    virtual void preRead(){}

    /** Reads the inherited object's data from the XML element */
    virtual void readData( QDomElement& element ) = 0;

    /** Provides the an API for committing post-read operations */
    virtual void postRead(){}

    /** Provides the an API for committing pre-write operations */
    virtual void preWrite(){}

    /** Writes the inherited object's data into the XML element */
    virtual void writeData( QDomElement& element ) = 0;

    /** Provides the an API for committing post-write operations */
    virtual void postWrite(){}

};
