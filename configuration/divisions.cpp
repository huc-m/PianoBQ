#include "configuration/tuneconfig.h"
#include "configuration/configurationconstant.h"

#include <QSettings>

#include "midi/globals.h"

QStringList readDivisions( ) {
    QSettings conf = QSettings( QDir::homePath() + ConfigFileName, QSettings::NativeFormat);
   return conf.value( "DIVISIONS/divisions" ).toStringList();
}

QStringList readNotUsedDivisions( ) {
    QSettings conf = QSettings( QDir::homePath() + ConfigFileName, QSettings::NativeFormat);
    QStringList divisionAll = conf.value( "DIVISIONS/divisions" ).toStringList();
    QStringList notUsed = divisionAll;
    conf.beginGroup( "TUNES" );
    for( QString division : divisionAll )
        for( QString key : conf.allKeys() ) if( conf.value( key ).toStringList()[1] == division ) {
            notUsed.removeOne( division );
            break;
        }
   return notUsed;
}

void delDivision( QString division )  {
    QSettings conf = QSettings( QDir::homePath() + ConfigFileName, QSettings::NativeFormat);
    conf.beginGroup( "DIVISIONS" );
    QStringList divisions = conf.value( "divisions" ).toStringList();
    divisions.removeOne( division );
    conf.setValue( "divisions", divisions );
}

void saveDivisions(  QStringList dlist){
    QSettings conf = QSettings( QDir::homePath() + ConfigFileName, QSettings::NativeFormat);
    dlist.sort();
    conf.setValue("DIVISIONS/divisions", dlist);
}

QString getDivisionByTuneName( MainWindow *mainwindow ) {

    QSettings conf = QSettings( QDir::homePath() + ConfigFileName, QSettings::NativeFormat);
    conf.beginGroup( "TUNES" );
    QStringList data = conf.value( mainwindow->curTuneName).toStringList();

    if( data.size() > 1 ) return data[1];
    else return QString();
}

