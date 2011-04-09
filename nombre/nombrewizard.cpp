#include "nombrewizard.h"
#include "ui_nombrewizard.h"

NombreWizard::NombreWizard(QWidget *parent) :
    QWizard(parent),
    ui(new Ui::NombreWizard)
{
    ui->setupUi(this);

    setOption( QWizard::DisabledBackButtonOnLastPage, true );

    connect( ui->targetFileBtn, SIGNAL(clicked()), SLOT(changeTargetFile()) );
    connect( ui->rootFolderBtn, SIGNAL(clicked()), SLOT(changeRootDirectory()) );
    connect( ui->newNameBtn, SIGNAL(clicked()), SLOT(changeNewFileName()) );
}

NombreWizard::~NombreWizard()
{
    delete ui;
}

bool NombreWizard::validateCurrentPage()
{
    if( currentPage() == ui->summaryPage )
    {
        if( QMessageBox::warning( this, "Really Rename", "Are you sure you want to proceed with rename?  \nChanges cannot be undone!", QMessageBox::Ok, QMessageBox::Cancel ) == QMessageBox::Cancel )
            return false;
        executeRename();
        return true;
    }
    else
        return true;
}

bool NombreWizard::copyFile( const QString& srcPath, const QString& dstPath )
{
    QFile srcFile( srcPath );
    QFile dstFile( dstPath );

    if( !srcFile.open( QFile::ReadOnly ) )
        return false;
    if( !dstFile.open( QFile::WriteOnly ) )
        return false;

    QTextStream inStream( &srcFile );
    QTextStream outStream( &dstFile );

    forever
    {
        QString inLine = inStream.readLine();
        if( inLine.isNull() )
            break;
        outStream << inLine << "\n";
    }

    return true;
}

bool NombreWizard::fileContains( const QRegExp& expr, const QString& srcPath )
{
    QFile srcFile( srcPath );

    if( !srcFile.open( QFile::ReadOnly ) )
        return false;

    QTextStream inStream( &srcFile );

    while( !inStream.atEnd() )
    {
        if( inStream.readLine().contains( expr ) )
            return true;
    }

    return false;
}

bool NombreWizard::replaceInstances( const QRegExp& expr, const QString& rplc, const QString& srcPath, const QString& dstPath )
{
    QFile srcFile( srcPath );
    QFile dstFile( dstPath );

    if( !srcFile.open( QFile::ReadOnly ) )
        return false;
    if( !dstFile.open( QFile::WriteOnly ) )
        return false;

    QTextStream inStream( &srcFile );
    QTextStream outStream( &dstFile );

    while( !inStream.atEnd() )
    {
        QString inLine = inStream.readLine();
        outStream << inLine.replace( expr, rplc ) << "\n";
    }

    return true;
}

void NombreWizard::log( const QString& text )
{
    ui->logBox->appendPlainText( text );
}

void NombreWizard::replaceInChildren( QDir& folder )
{

    QString dlms;
    if( ui->quoteDelimiterChkBox->isChecked() )
        dlms += "\"'";
    if( ui->whitespaceDelimiterChkBox->isChecked() )
        dlms += "\\s";

    // Search and replace for the file name (with no path data)
    QString namePattern = QString( "([%1])(%2)([%3])" )
                          .arg( dlms )
                          .arg( targetFile.fileName() )
                          .arg( dlms )
                          .replace( ".", "\\." );
    QRegExp nameRegExpr( namePattern );
    QString nameReplace = "\\1" + newFile.fileName() + "\\3";

    // Search and replace for the absolute file path
    QString absolutePattern = QString( "([%1])(%2)([%3])" )
                          .arg( dlms )
                          .arg( targetFile.absoluteFilePath() )
                          .arg( dlms )
                          .replace( ".", "\\." );
    QRegExp absoluteRegExpr( absolutePattern );
    QString absoluteReplace = "\\1" + newFile.absoluteFilePath() + "\\3";

    // Search and replace for the a relative file path
    QString relativePattern = QString( "([%1])(%2)([%3])" )
                          .arg( dlms )
                          .arg( folder.relativeFilePath( targetFile.absoluteFilePath() ) )
                          .arg( dlms )
                          .replace( ".", "\\." );
    QRegExp relativeRegExpr( relativePattern );
    QString relativeReplace = "\\1" + folder.relativeFilePath( newFile.absoluteFilePath() ) + "\\3";

    bool live = true;

    log( "" );
    log( "========================================" );
    log( "Searching for replacements in directory:" );
    log( QString( "  %1" ).arg( folder.absolutePath() ) );
    log( "----------------------------------------" );
    QStringList children = folder.entryList( QStringList(), QDir::Dirs |
                                                            QDir::Files |
                                                            QDir::NoDotAndDotDot |
                                                            QDir::NoSymLinks |
                                                            QDir::Hidden );

    log( QString( "  Found %1 files and sub-directories" ).arg( children.size() ) );

    foreach( const QString child, children )
    {
        QString temp = "." + child + ".old";

        QFileInfo childInfo( folder, child );
        QFileInfo tempInfo(  folder, temp );

        QString childPath = childInfo.absoluteFilePath();
        QString tempPath  = tempInfo.absoluteFilePath();

        if( childInfo.isDir() )
        {
            log( "" );
            log( "  Found A Sub-directory:" );
            log( QString( "    %1" ).arg( childPath ) );
            if( !childInfo.isReadable() )
                log( "    Skipping unreadable sub-directory" );
            else if( ui->svnFolderChkBox->isChecked() && child == ".svn" )
                log( "    Skipping Subversion sub-directory" );
            else
            {
                QDir newParent( childPath );
                replaceInChildren( newParent );
            }
        }
        else
        {
            if( fileContains( nameRegExpr, childPath ) )
            {
                log( "" );
                log( "  Found File with file name match:" );
                log( QString( "    %1" ).arg( childPath ) );
                log( QString( "    %1" ).arg( QString( "-" ).repeated( childPath.size() ) ) );

                /// @todo  if a back up already exists, choose a new name (sequential)
                if( live = copyFile( childPath, tempPath ) )
                {
                    log( "    Created backup:" );
                    log( QString( "      %1" ).arg( tempPath ) );
                }
                else
                    log( "    Backup Creation Failed" );

                if( live = live && replaceInstances( nameRegExpr, nameReplace, tempPath, childPath ) )
                    log( "    Name Replacement Successfull!" );
                else
                    log( "    Replacement failed" );

                if( ui->deleteTempsChkBox->isChecked() && folder.remove( tempPath ) )
                    log( "    Removed backup" );
            }

            if( fileContains( absoluteRegExpr, childPath ) )
            {
                log( "" );
                log( "  Found File with absolute path match:" );
                log( QString( "    %1" ).arg( childPath ) );
                log( QString( "    %1" ).arg( QString( "-" ).repeated( childPath.size() ) ) );

                if( live = copyFile( childPath, tempPath ) )
                {
                    log( "    Created backup:" );
                    log( QString( "      %1" ).arg( tempPath ) );
                }
                else
                    log( "    Backup Creation Failed" );

                if( live = live && replaceInstances( absoluteRegExpr, absoluteReplace, tempPath, childPath ) )
                    log( "    Replacement Successfull!" );
                else
                    log( "    Replacement failed" );

                if( ui->deleteTempsChkBox->isChecked() && folder.remove( tempPath ) )
                    log( "    Removed backup" );
            }

            if( fileContains( relativeRegExpr, childPath ) )
            {
                log( "" );
                log( "  Found File with file relative path match:" );
                log( QString( "    %1" ).arg( childPath ) );
                log( QString( "    %1" ).arg( QString( "-" ).repeated( childPath.size() ) ) );

                if( live = copyFile( childPath, tempPath ) )
                {
                    log( "    Created backup:" );
                    log( QString( "      %1" ).arg( tempPath ) );
                }
                else
                    log( "    Backup Creation Failed" );

                if( live = live && replaceInstances( relativeRegExpr, relativeReplace, tempPath, childPath ) )
                    log( "    Replacement Successfull!" );
                else
                    log( "    Replacement failed" );

                if( ui->deleteTempsChkBox->isChecked() && folder.remove( tempPath ) )
                    log( "    Removed backup" );
            }
        }
    }
}

void NombreWizard::executeRename()
{
    ui->logBox->clear();

    bool success = true;

    log( "Attempting to rename file" );
    log( "-------------------------" );

    if( ui->svnRenameChkBox->isChecked() )
    {
        QProcess svnRename( this );
        QStringList args;
        args << "rename" << targetFile.absoluteFilePath() << newFile.absoluteFilePath();
        svnRename.start( "svn", args );
        svnRename.waitForFinished();

        if( success = svnRename.exitStatus() == QProcess::NormalExit )
            log( "SVN Rename Command Successful:" );
        else
            log( "Svn Rename Command Failed:" );

        foreach( const QString& line, QString( svnRename.readAll() ).split( '\n' ) )
            log( QString( "  > %1" ).arg( line.trimmed() ) );
    }
    else
    {
        if( success = targetFile.dir().rename( targetFile.absoluteFilePath(), newFile.absoluteFilePath() ) )
        {
            log( "File Renamed" );
            log( QString( "  From: %1" ).arg( targetFile.absoluteFilePath()) );
            log( QString( "    To: %1" ).arg( newFile.absoluteFilePath()) );
        }
        else
            log( "File rename failed" );
    }

    log( "" );
    log( "==========================================" );
    log( "Beginning Replace process in selected root" );
    log( QString( "  %1" ).arg( rootDir.absolutePath() ) );

    if( success )
        replaceInChildren( rootDir );

    log( "" );
    log( "========================" );
    log( "Finished Replace process" );


}

void NombreWizard::updateTargetFileName()
{
    ui->targetFileBox1->setText( targetFile.fileName() );
    ui->targetFileBox2->setText( targetFile.fileName() );
    ui->targetFileBox3->setText( targetFile.fileName() );
    ui->targetFileBox4->setText( targetFile.fileName() );
}

void NombreWizard::updateRootDirectoryName()
{
    ui->rootFolderBox2->setText( rootDir.absolutePath() );
    ui->rootFolderBox3->setText( rootDir.absolutePath() );
    ui->rootFolderBox4->setText( rootDir.absolutePath() );
}

void NombreWizard::updateNewFileName()
{
    QString relativePath = targetFile.dir().relativeFilePath( newFile.absoluteFilePath() );
    ui->newNameBox3->setText( relativePath );
    ui->newNameBox4->setText( relativePath );
}

void NombreWizard::changeTargetFile()
{
    QString fileName = QFileDialog::getOpenFileName( this, "Select a file to rename" );
    if( fileName == QString() )
        return;

    targetFile = QFileInfo( fileName );
    rootDir = targetFile.dir();
    updateTargetFileName();
    updateRootDirectoryName();
}

void NombreWizard::changeRootDirectory()
{
    QString dirName = QFileDialog::getExistingDirectory( this, "Select a root folder" );
    if( dirName == QString() )
        return;
    rootDir = QDir( dirName );
    updateRootDirectoryName();
}

void NombreWizard::changeNewFileName()
{
    QString fileName = QFileDialog::getSaveFileName( this, "Select a new file name", targetFile.absolutePath() );
    if( fileName == QString() )
        return;
    newFile = QFileInfo( fileName );
    updateNewFileName();
}
