#pragma once

#include <QFileSystemModel>
#include <QSet>

class CustomModel : public QFileSystemModel
{
    Q_OBJECT

public:
    CustomModel( QObject* parent = NULL );
    virtual ~CustomModel();

    virtual QVariant data( const QModelIndex& index, int role ) const;
    virtual Qt::ItemFlags flags( const QModelIndex& index ) const;
    virtual bool setData( const QModelIndex& index, const QVariant& value, int role );

    bool isCheckable( const QModelIndex& index ) const;
    bool isChecked( const QModelIndex& index ) const;
    bool setChecked( const QModelIndex& index, bool checked );

protected:
    QSet<QPersistentModelIndex> checkList;

};
