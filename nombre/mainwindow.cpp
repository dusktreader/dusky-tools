#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>
using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect( ui->kickBtn, SIGNAL(clicked()), SLOT(kickOutTheJams()) );
}

MainWindow::~MainWindow()
{
    /*
    delete fsMdl;
    delete checkAllChildrenAction;
    delete uncheckAllChildrenAction;
    delete checkAllAction;
    delete uncheckAllAction;
    */
    delete ui;
}

void MainWindow::kickOutTheJams()
{
    NombreWizard wiz( this );
    wiz.exec();
}

/*
void MainWindow::init()
{

    fsMdl = new CustomModel( this );
    fsMdl->setRootPath( "/" );

    fsView = ui->fsView;
    fsView->setModel( fsMdl );
    for( int i=1; i<fsMdl->columnCount(); i++ )
        fsView->hideColumn( i );
    fsView->expandAll();
    fsView->collapseAll();

    QFileInfo rootDir( fsMdl->rootPath() );
    setWindowTitle( rootDir.absoluteFilePath() );
    setContextMenuPolicy( Qt::CustomContextMenu );

    rootIndex = fsView->rootIndex();

    checkAllChildrenAction = new QAction( "Check Contained Files", fsView );
    uncheckAllChildrenAction = new QAction( "Uncheck Contained Files", fsView );

    checkAllAction = new QAction( "Check All", fsView );
    uncheckAllAction = new QAction( "Uncheck All", fsView );

    connect(   checkAllChildrenAction, SIGNAL(triggered()), SLOT(  checkAllChildren()) );
    connect( uncheckAllChildrenAction, SIGNAL(triggered()), SLOT(uncheckAllChildren()) );

    connect(   checkAllAction, SIGNAL(triggered()), SLOT(  checkAll()) );
    connect( uncheckAllAction, SIGNAL(triggered()), SLOT(uncheckAll()) );

    connect( fsView, SIGNAL(doubleClicked(QModelIndex)), SLOT(adjustRoot(QModelIndex)) );
    connect( fsView, SIGNAL(customContextMenuRequested(const QPoint&)), SLOT(showContextMenu(const QPoint&)) );
    connect( ui->upBtn, SIGNAL(clicked()), SLOT(moveUp()) );

}

void MainWindow::showContextMenu( const QPoint& pt )
{
    QList<QAction*> actions;

    actionIndex = fsView->indexAt( pt );

    if( !actionIndex.isValid() )
    {
        ui->statusBar->showMessage( "Invalid Index", 3000 );
        return;
    }


    actions << checkAllAction << uncheckAllAction << checkAllChildrenAction << uncheckAllChildrenAction;

    if( fsMdl->isDir( actionIndex ) )
    {
        checkAllChildrenAction->setEnabled( true );
        uncheckAllChildrenAction->setEnabled( true );
    }
    else
    {
        checkAllChildrenAction->setEnabled( false );
        uncheckAllChildrenAction->setEnabled( false );
    }

    QMenu::exec( actions, fsView->mapToGlobal( pt ) );
}

void MainWindow::checkAllChildren()
{
    setCheckedAll( actionIndex, true );
}

void MainWindow::uncheckAllChildren()
{
    setCheckedAll( actionIndex, false );
}

void MainWindow::checkAll()
{
    setCheckedAll( fsView->rootIndex(), true );
}

void MainWindow::uncheckAll()
{
    setCheckedAll( fsView->rootIndex(), false );
}

void MainWindow::setCheckedAll( const QModelIndex& index, bool checked )
{
    cout << "check all for " << fsMdl->fileInfo(index).filePath().toStdString() << endl;
    cout << "  this idiot has " << fsMdl->rowCount(index) << " kids: " << fsMdl->hasChildren( index ) << endl;
    int i = 0;
    QModelIndex childIndex = index.child( i, 0 );

    while( childIndex.isValid() )
    {
        if( fsMdl->isDir(childIndex) )
        {
            cout << "recursing on " << fsMdl->fileInfo(childIndex).filePath().toStdString() << endl;
            setCheckedAll( childIndex, checked );
        }
        else
        {
            cout << "setting " << fsMdl->fileInfo(childIndex).filePath().toStdString() << endl;
            fsMdl->setChecked( childIndex, checked );
        }
        childIndex = index.child( ++i, 0 );
    }
}

void MainWindow::adjustRoot( QModelIndex index )
{
    QFileInfo sel = fsMdl->fileInfo( index );

    if( !sel.isDir() )
    {
        ui->statusBar->showMessage( "not a directory", 3000 );
        return;
    }

    if( !sel.isReadable() )
    {
        ui->statusBar->showMessage( "not readable", 3000 );
        return;
    }

    fsView->setRootIndex( index );
    rootIndex = index;
    setWindowTitle( sel.absoluteFilePath() );
}

void MainWindow::moveUp()
{
    QModelIndex currIndex = fsView->rootIndex();
    QModelIndex parentIndex = fsMdl->parent( currIndex );
    if( parentIndex.isValid() )
    {
        fsView->setRootIndex( parentIndex );
        rootIndex = parentIndex;
        setWindowTitle( fsMdl->fileInfo( parentIndex ).absoluteFilePath() );
    }
    else
        ui->statusBar->showMessage( "Invalid Index", 3000 );
}
*/
