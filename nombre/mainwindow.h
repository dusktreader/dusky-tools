#pragma once

#include <QMainWindow>
#include <QFileSystemModel>
#include <QFileInfo>
#include <QTreeView>
#include <QDir>
#include <QDirIterator>

//#include "custommodel.h"
#include "nombrewizard.h"

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    /*
    void adjustRoot( QModelIndex index );
    void moveUp();
    void showContextMenu( const QPoint& pt );
    void checkAllChildren();
    void uncheckAllChildren();
    void checkAll();
    void uncheckAll();
    */

    void kickOutTheJams();

private:
    Ui::MainWindow *ui;

    /*
    CustomModel* fsMdl;
    QTreeView* fsView;

    QAction* checkAllChildrenAction;
    QAction* uncheckAllChildrenAction;
    QAction* checkAllAction;
    QAction* uncheckAllAction;

    QModelIndex actionIndex;
    QPersistentModelIndex rootIndex;

    void init();
    void setCheckedAll( const QModelIndex& index, bool checked );
    */
};

