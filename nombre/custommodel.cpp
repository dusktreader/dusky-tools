#include "custommodel.h"

#include <iostream>
using namespace std;

CustomModel::CustomModel( QObject* parent ) : QFileSystemModel( parent ){}

CustomModel::~CustomModel(){}

QVariant CustomModel::data( const QModelIndex& index, int role ) const
{
    /*
    cout << "fetching " << flush;
    switch( role )
    {
    case Qt::DisplayRole       : cout << "DisplayRole" << flush; break;
    case Qt::DecorationRole    : cout << "DecorationRole" << flush; break;
    case Qt::EditRole          : cout << "EditRole" << flush; break;
    case Qt::ToolTipRole       : cout << "ToolTipRole" << flush; break;
    case Qt::StatusTipRole     : cout << "StatusTipRole" << flush; break;
    case Qt::WhatsThisRole     : cout << "WhatsThisRole" << flush; break;
    case Qt::SizeHintRole      : cout << "SizeHintRole" << flush; break;
    case Qt::FontRole          : cout << "FontRole" << flush; break;
    case Qt::TextAlignmentRole : cout << "TextAlignmentRole" << flush; break;
    case Qt::BackgroundRole    : cout << "BackgroundRole" << flush; break;
    case Qt::ForegroundRole    : cout << "ForegroundRole" << flush; break;
    case Qt::CheckStateRole    : cout << "CheckStateRole" << flush; break;
    default                    : cout << "UnkownRole" << flush; break;
    }
    cout << " for " << fileInfo(index).filePath().toStdString() << endl;
    */

    if( role == Qt::CheckStateRole && isCheckable(index) )
    {
        /*
        cout << " check state for " << fileInfo(index).filePath().toStdString()
             << " is " << ( checkList.contains(index) ? "Checked" : "Unchecked" ) << endl;
         */
        return checkList.contains(index) ? Qt::Checked : Qt::Unchecked;
    }
    else
        return QFileSystemModel::data( index, role );
}

Qt::ItemFlags CustomModel::flags( const QModelIndex& index ) const
{
    Qt::ItemFlags itemFlags = QFileSystemModel::flags( index );
    if( isCheckable(index) )
        itemFlags |= Qt::ItemIsUserCheckable;
    return itemFlags;
}

bool CustomModel::setData( const QModelIndex& index, const QVariant& value, int role )
{
    if( role == Qt::CheckStateRole )
    {
        if( !isCheckable(index) )
            return false;

        if( value == Qt::Checked )
            checkList.insert(index);
        else
            checkList.remove(index);
        emit dataChanged( index, index );
        return true;
    }
    else
        return QFileSystemModel::setData( index, value, role );
}

bool CustomModel::isCheckable( const QModelIndex& index ) const
{
    QFileInfo itemInfo = fileInfo( index );
    return !itemInfo.isDir() && itemInfo.isReadable() && itemInfo.isWritable();
}

bool CustomModel::isChecked( const QModelIndex& index ) const
{
    return checkList.contains( index );
}

bool CustomModel::setChecked( const QModelIndex& index, bool checked )
{
    if( !isCheckable(index) )
        return false;

    if( checked )
        checkList.insert( index );
    else
        checkList.remove( index );
    emit dataChanged( index, index );
    return true;
}
