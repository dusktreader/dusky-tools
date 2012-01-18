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
