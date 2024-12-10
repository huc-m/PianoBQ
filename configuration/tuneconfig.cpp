#include "configuration/tuneconfig.h"
#include <QSettings>

#include "midi/globals.h"
#include "midi/midi_with_fluidsynth.h"

#include "mainwindow.h"
#include "ui_mainwindow.h"

extern QSettings *tune_conf;
extern QSettings *tunes_conf;

void saveTune(QString fileName, QString division){
    QString fileNumber;
    fileNumber.setNum( tunes_conf->value("OTHER/LastFileNumber").toInt() + 1 );
    tunes_conf->setValue("OTHER/LastFileNumber", fileNumber);
    tunes_conf->beginGroup( "TUNES" );
        tunes_conf->setValue(mainwindow->windowTitle(), QStringList( { fileName, division, QString::number( left_hand_channel ), QString::number( right_hand_channel ),  fileNumber } ));
    tunes_conf->endGroup();
}

QStringList getTunesByDivision( QString division ){
    tunes_conf->beginGroup( "TUNES" );
        QStringList list;
        for( QString key : tunes_conf->allKeys() ) if( tunes_conf->value( key ).toStringList()[1] == division ) list.append( key );
    tunes_conf->endGroup();
    return list;
}

QString getTuneFile() {
    tunes_conf->beginGroup( "TUNES" );
        QStringList data = tunes_conf->value( mainwindow->windowTitle() ).toStringList();
    tunes_conf->endGroup();

    if( data.size() < 4 ) return QString();
    left_hand_channel = data[2].toInt();
    right_hand_channel = data[3].toInt();
    return data[0];
}

void saveTunePart( QString partName ) {
    if( tune_conf == NULL ) return;
    tune_conf->beginGroup( "PARTS" );
        tune_conf->setValue( partName,  QStringList( { QString::number( cur_start ), QString::number( cur_finish )}));
    tune_conf->endGroup();
}

QStringList getPartsByTune() {
    if( tune_conf == NULL ) return QStringList();
    tune_conf->beginGroup( "PARTS" );
        QStringList list = tune_conf->allKeys();
    tune_conf->endGroup();
    return list;

}

void setStartFinishByPart( QString partName ) {
    if( tune_conf == NULL ) return;
    tune_conf->beginGroup( "PARTS" );
        QStringList data = tune_conf->value( partName ).toStringList();
    tune_conf->endGroup();
    if( data.size() < 1 ) return;
    cur_start = data[0].toInt();
    cur_finish = data[1].toInt();
}

void init_tune_conf(){
    delete tune_conf; tune_conf = NULL;
    if( mainwindow->windowTitle().isEmpty() ) return;
    tunes_conf->beginGroup("TUNES");
        QStringList data = tunes_conf->value( mainwindow->windowTitle()).toStringList();
        tune_conf = new QSettings( QDir::homePath() + CONFIG_TUNES_DIRECTORY + data[4], QSettings::NativeFormat );
        left_hand_channel = data[2].toInt();
        right_hand_channel = data[3].toInt();
    tunes_conf->endGroup();
    tune_conf->beginGroup( "STAFF" );
        mainwindow->staff_step = tune_conf->value( "step" , mainwindow->staff_step_default ).toInt();
        mainwindow->staff_pading_h = tune_conf->value( "top", mainwindow->staff_pading_h_default ).toInt();
    tune_conf->endGroup();
    mainwindow->setStaffParameters();
    reset_keyboard_fluid( -1 );
    set_hand( ALL_H );
    mainwindow->ui->handAllHandsAction->setChecked( true );
}