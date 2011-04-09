#include "qtools.h"

bool qstr2bool( const QString& str )
{
    return QString( "ty" ).contains( str.toLower().at(0) );
}

QString bool2qstr( bool val )
{
    return val ? "True" : "False";
}

void setDiff( const QStringList& newList, const QStringList& oldList, QStringList& addList, QStringList& remList )
{
    QSet<QString> newSet = QSet<QString>::fromList( newList );
    QSet<QString> oldSet = QSet<QString>::fromList( oldList );
    QSet<QString> addSet = newSet - oldSet;
    QSet<QString> remSet = oldSet - newSet;
    addList = addSet.toList();
    remList = remSet.toList();
}

void meanStdvPrsd( const QVector<double>& vals, double& mean, double& stdv, double& prsd, bool nm1d )
{
    meanStdvPrsd( vals.toStdVector(), mean, stdv, prsd, nm1d );
}
