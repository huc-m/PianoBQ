#include "configuration/tuneconfig.h"
#include "midi/constants.h"
#include "mainwindow.h"

#include "midi/globals.h"

extern QSettings *tune_conf;
extern QSettings *tunes_conf;

QStringList readDivisions( ) {
   return tunes_conf->value( "DIVISIONS/divisions" ).toStringList();
}

QStringList readNotUsedDivisions( ) {
    if( tune_conf == NULL ) return QStringList();
    QStringList divisionAll = tunes_conf->value( "DIVISIONS/divisions" ).toStringList();
    QStringList notUsed = divisionAll;
    tunes_conf->beginGroup( "TUNES" );
        for( QString division : divisionAll )
            for( QString key : tunes_conf->allKeys() ) if( tunes_conf->value( key ).toStringList()[1] == division ) {
                notUsed.removeOne( division );
                break;
            }
    tunes_conf->endGroup();
   return notUsed;
}

void delDivision( QString division )  {
    tunes_conf->beginGroup( "DIVISIONS" );
        QStringList divisions = tunes_conf->value( "divisions" ).toStringList();
        divisions.removeOne( division );
        tunes_conf->setValue( "divisions", divisions );
    tunes_conf->endGroup();
}

void saveDivisions(  QStringList dlist){
    dlist.sort();
    tunes_conf->setValue("DIVISIONS/divisions", dlist);
}

QString getDivisionByTuneName() {
    tunes_conf->beginGroup( "TUNES" );
        QStringList data = tunes_conf->value( mainwindow->windowTitle() ).toStringList();
    tunes_conf->endGroup();
    if( data.size() > 1 ) return data[1]; else return QString();
}