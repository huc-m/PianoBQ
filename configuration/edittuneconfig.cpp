#include "configuration/edittuneconfig.h"

#include "midi/globals.h"
#include "mainwindow.h"

#include <QFile>

extern QSettings *tune_conf;
extern QSettings *tunes_conf;

void editTuneConfig( QString newTuneMame, QString newDivision, int newLHch, int newRHch ) {

    tunes_conf->beginGroup( "TUNES" );
        QStringList data = tunes_conf->value( mainwindow->windowTitle() ).toStringList();
        data[1] = newDivision;
        data[2] = QString::number( newLHch );
        data[3] = QString::number( newRHch );
        tunes_conf->remove( mainwindow->windowTitle() );
        tunes_conf->setValue( newTuneMame, data );

        left_hand_channel = newLHch;
        right_hand_channel = newRHch;
        mainwindow->setWindowTitle( newTuneMame );
    tunes_conf->endGroup();
}

void deleteTune() {
    tunes_conf->beginGroup( "TUNES" );
        tunes_conf->remove( mainwindow->windowTitle() );
    tunes_conf->endGroup();
    if( tune_conf != NULL ) {
        QFile( tune_conf->fileName() ).remove();
        delete tune_conf;
        tune_conf = NULL;
    }
    mainwindow->setWindowTitle( QString() );
}

void deletePart( QString part ) {
    if( tune_conf == NULL ) return;
    tune_conf->beginGroup( "PARTS" );
        tune_conf->remove( part );
        tune_conf->sync();
    tune_conf->endGroup();
    if( QFile(tune_conf->fileName()).size() < 3 ) QFile(tune_conf->fileName()).remove();
    mainwindow->setToolboxParts();
}


void saveStaffForm() {
    if( tune_conf == NULL ) return;
    if( mainwindow->staff_pading_h == mainwindow->staff_pading_h_default && mainwindow->staff_step == mainwindow->staff_step_default){
        tune_conf->remove( "STAFF" );
        tune_conf->sync();
        if( QFile(tune_conf->fileName()).size() < 3 ) QFile(tune_conf->fileName()).remove();
    } else {
        tune_conf->beginGroup( "STAFF" );
            tune_conf->setValue( "top", mainwindow->staff_pading_h );
            tune_conf->setValue( "step", mainwindow->staff_step );
        tune_conf->endGroup();
    }
}