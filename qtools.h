#pragma once

#include <QtDebug>
#include <QMessageBox>
#include <QStringList>
#include <QList>
#include <QSet>
#include "tools.h"

/// @todo  Add a sorted list class

#define QCATCH_LASS                                                          \
    catch( LocalAssert& err )                                                \
    {                                                                        \
        QMessageBox warning;                                                 \
        warning.setIcon( QMessageBox::Warning );                             \
        warning.setText( err.what() );                                       \
        warning.setDetailedText( QString::fromStdString(err.details()) );    \
        warning.exec();                                                      \
    }                                                                        \
    catch( ... )                                                             \
    {                                                                        \
        QMessageBox warning;                                                 \
        warning.setIcon( QMessageBox::Critical );                            \
        warning.setText( "An unknown Exception occurred.  Re-throwing..." ); \
        warning.exec();                                                      \
        throw;                                                               \
    }

#define ASSERT_QMSG( expr, msg ) ASSERT_MSG( expr, msg.toStdString() )

    /** Prints the file and line */
#define QDB_REP { qDebug() << __FILE__ << ":" << __LINE__ << ": reporting"; }

    /** Prints the file name, line and a message
      * @param  msg - The message to print
      */
#define QDB_REP_MSG( msg ) { qDebug() << __FILE__ ":" << __LINE__ << ": message: " << (msg); }

/** Prints the file, line, variable name, and variable value
 * @param  var - The variable to report
 */
    #define QDB_REP_VAR( var ) { qDebug() << __FILE__ ":" << __LINE__ << ": " << #var << " = " << var; }

bool qstr2bool( const QString& str );
QString bool2qstr( bool val );

#include <QTime>
#include <QMap>
class StopWatch
{
private:
    enum runMode{ RUNNING, STOPPED };

    QMap<QString,QTime> currentTimes;
    QMap<QString,int> totalTimes;
    QMap<QString,runMode>  runModes;

public:

    StopWatch()
    {
        addTimer( "default" );
    }

    void addTimer( const QString& timerName, bool startNow=false )
    {
        ASSERT_MSG( !currentTimes.contains( timerName ), QString( "A timer of that name already exists: %1" ).arg( timerName ).toStdString() );
        currentTimes[timerName] = QTime();
        totalTimes[timerName] = 0;
        runModes[timerName] = STOPPED;
        if( startNow )
            startTimer( timerName );
    }

    void startTimer( const QString& timerName="default", bool restart=true )
    {
        ASSERT_MSG( currentTimes.contains( timerName ), QString( "No matching timer found: %1" ).arg( timerName ).toStdString() );
        ASSERT_MSG( runModes[timerName] == STOPPED, "Timer is already running" );

        if( restart )
            totalTimes[timerName] = 0;
        currentTimes[timerName].restart();
        runModes[timerName] = RUNNING;
    }

    void stopTimer( const QString& timerName="default" )
    {
        ASSERT_MSG( currentTimes.contains( timerName ), QString( "No matching timer found: %1" ).arg( timerName ).toStdString() );
        ASSERT_MSG( runModes[timerName] == RUNNING, "Timer is not running" );

        totalTimes[timerName] += currentTimes[timerName].elapsed();
        runModes[timerName] = STOPPED;
    }

    int elapsedTime( const QString& timerName="default" )
    {
        ASSERT_MSG( currentTimes.contains( timerName ), QString( "No matching timer found: %1" ).arg( timerName ).toStdString() );
        int totalTime = totalTimes[timerName];
        if( runModes[timerName] == RUNNING )
            totalTime += currentTimes[timerName].elapsed();
        return totalTime;
    }

    QString printElapsedTime( const QString& timerName="default" )
    {
        return timerName + ": " + QTime(0,0).addMSecs( elapsedTime( timerName ) ).toString( "hh:mm:ss.zzz" );
    }
};

void setDiff( const QStringList& newList, const QStringList& oldList, QStringList& addList, QStringList& remList );

void meanStdvPrsd( const QVector<double> &vals, double &mean, double &stdv, double &prsd, bool nm1d=true );
