#include "persist.h"

QString PersistXML::_formatVersionName = "Adam";



QString PersistXML::alias() const
{
    ASSERT_MSG( this->metaObject() != NULL, "Cannot fetch alias information for a NULL meta object" );
    return this->metaObject()->className();
}



void PersistXML::save( QIODevice& saveDevice, QString saveTitle )
{
    QDomDocument xmlDoc( saveTitle );

    // If at any point invalid data is attemepted to be written, the node will be returned as NULL
    xmlDoc.implementation().setInvalidDataPolicy( QDomImplementation::ReturnNullNode );

    QDomElement root = xmlDoc.createElement( "root" );
    xmlDoc.appendChild( root );

    root.setAttribute( "FormatVersionName", _formatVersionName );

    write( root, saveTitle );

    QTextStream outStream( &saveDevice );
    /// @todo: add checks to verify the coherence of the output stream
    xmlDoc.save( outStream, 4 );
}



void PersistXML::load( QIODevice& loadDevice, QString loadTitle )
{
    QDomDocument xmlDoc( loadTitle );

    QString errorString;
    int errorLine, errorColumn;
    ASSERT_QMSG( xmlDoc.setContent( &loadDevice, &errorString, &errorLine, &errorColumn ),
                 QString( "XML read failed at line %1, column %2: %3" )
                 .arg( errorLine )
                 .arg( errorColumn )
                 .arg( errorString ) );

    QDomElement root = xmlDoc.firstChildElement( "root" );
    root.setAttribute( "FormatVersionName", _formatVersionName );
    ASSERT_MSG( !root.isNull(), "Couldn't find root node" );
    ASSERT_MSG( root.attribute( "FormatVersionName" ) == _formatVersionName, "Outdated or invalid format" );
    read( root, loadTitle );
}



void PersistXML::read( QDomElement parent, QString tag )
{
    preRead();
    QDomElement child = fetchChild( parent, tag, alias() );
    readData( child );
    postRead();
}



void PersistXML::write( QDomElement& parent, QString tag )
{
    preWrite();
    QDomElement child = createChild( parent, tag, alias() );
    writeData( child );
    parent.appendChild( child );
    postWrite();
}



QDomElement PersistXML::fetchChild( const QDomElement& parent, const QString& tag, const QString& nodeType )
{
    QDomElement element = parent.firstChildElement( tag );
    ASSERT_QMSG( !element.isNull(),
                 QString( "Couldn't find a node matching the given tag: %1" ).arg( tag ) );
    ASSERT_QMSG( nodeType == "" || element.attribute( "NodeType" ) == nodeType,
                 QString( "Child with tag %1 has node type %2.  Expected %3" )
                 .arg( tag )
                 .arg( element.attribute( "NodeType" ) )
                 .arg( nodeType ) );
    return element;
}



QDomElement PersistXML::createChild( QDomElement& parent, QString tag, QString nodeType )
{
    QDomElement child = parent.firstChildElement( tag );

    // If there is already a node matching that tag, it is replaced.
    if( !child.isNull() )
        parent.removeChild( child );

    child = parent.ownerDocument().createElement( tag );
    ASSERT_MSG( !child.isNull(), "Couldn't create child node" );

    child.setAttribute( "NodeType", nodeType );
    ASSERT_MSG( !parent.appendChild( child ).isNull(), "Coluldn't append the child node" );

    return child;
}



QMap<QString, QVariant> PersistXML::readVariantMap( QDomElement parent, QString tag )
{
    QDomElement child = fetchChild( parent, tag, "QMap<QString, QVariant>" );

    QMap<QString, QVariant> persistanceMap;
    QDomNodeList mapItems = child.childNodes();

    bool ok = true;

    for( int i=0; i<mapItems.count(); i++ )
    {
        QDomElement mapItem = mapItems.item( i ).toElement();
        QString mapItemTag = mapItem.tagName();
        if( !mapItemTag.startsWith( MAP_KEY_PREFIX ) )
            continue;
        QString itemKey   = mapItemTag.remove( MAP_KEY_PREFIX );
        QString itemType  = mapItem.attribute( "type" );
        QString itemValueString = mapItem.attribute( "value" );
        QVariant itemValue;
        if( itemType == QVariant::typeToName( QVariant::Int ) )
            itemValue = QVariant( itemValueString.toInt( &ok ) );
        else if( itemType == QVariant::typeToName( QVariant::Double ) )
            itemValue = QVariant( itemValueString.toDouble( &ok ) );
        else if( itemType == QVariant::typeToName( QVariant::String ) )
            itemValue = QVariant( itemValueString );
        else
            ABORT_MSG( "Type is not registered for PersistXML QMap<QString, QVariant> read" );
        ASSERT_MSG( ok, "Couldn't convert map value" );

        persistanceMap[itemKey] = itemValue;
    }
    return persistanceMap;
}

void PersistXML::writeVariantMap( QDomElement& parent, QString tag, QMap<QString, QVariant> persistanceMap )
{
    QDomElement child = createChild( parent, tag, "QMap<QString, QVariant>" );

    foreach( QString itemKey, persistanceMap.keys() )
    {
        QVariant itemValue = persistanceMap[itemKey];
        QString itemTag = QString( "%1%2" ).arg( MAP_KEY_PREFIX ).arg( itemKey );
        QDomElement mapItem = child.ownerDocument().createElement( itemTag );
        mapItem.setAttribute( "type",  itemValue.typeName() );
        mapItem.setAttribute( "value", itemValue.toString() );
        child.appendChild( mapItem );
    }
    parent.appendChild( child );
}



QVector<QVariant> PersistXML::readVariantVector( QDomElement parent, QString tag )
{
    QDomElement child = fetchChild( parent, tag, "QVector<QVariant>" );

    bool ok = true;

    int count = child.attribute( "count" ).toInt( &ok );
    ASSERT_MSG( ok, "Couldn't extract vector size" );
    QVector<QVariant> variantVector = QVector<QVariant>( count );

    for( int i=0; i<count; i++ )
    {
        QString itemTag = QString( "%1%2" ).arg( VECTOR_INDEX_PREFIX ).arg( i );

        QDomElement vectorItem = child.firstChildElement( itemTag );
        ASSERT_MSG( !vectorItem.isNull(), "Vector is missing an element" );

        QString itemType  = vectorItem.attribute( "type" );
        QString itemValueString = vectorItem.attribute( "value" );
        QVariant itemValue;
        if( itemType == QVariant::typeToName( QVariant::Int ) )
            itemValue = QVariant( itemValueString.toInt( &ok ) );
        else if( itemType == QVariant::typeToName( QVariant::Double ) )
            itemValue = QVariant( itemValueString.toDouble( &ok ) );
        else if( itemType == QVariant::typeToName( QVariant::String ) )
            itemValue = QVariant( itemValueString );
        else
            ABORT_MSG( "Type is not registered for PersistXML QVector<QVariant> read" );
        ASSERT_MSG( ok, "Couldn't convert map value" );

        variantVector[i] = itemValue;
    }
    return variantVector;
}



void PersistXML::writeVariantVector( QDomElement& parent, QString tag, QVector<QVariant> variantVector )
void PersistXML::writeVariantVector( QDomElement& parent, QString tag, QVector<QVariant> persistanceVector )
{
    QDomElement child = createChild( parent, tag, "QVector<QVariant>" );
    child.setAttribute( "count", variantVector.size() );

    for( int i=0; i<variantVector.size(); i++ )
    {
        QVariant itemValue = variantVector[i];
        QString itemTag = QString( "%1%2" ).arg( VECTOR_INDEX_PREFIX ).arg( i );
        QDomElement vectorItem = child.ownerDocument().createElement( itemTag );
        vectorItem.setAttribute( "type",  itemValue.typeName() );
        vectorItem.setAttribute( "value", itemValue.toString() );
        child.appendChild( vectorItem );
    }
    parent.appendChild( child );
}


