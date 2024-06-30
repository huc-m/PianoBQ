#include "configuration/edittuneconfig.h"
#include "configuration/configurationconstant.h"

#include "midi/globals.h"
#include "midi/midi_with_fluidsynth.h"

#include <QSettings>
#include <QFile>


void editTuneConfig( MainWindow* mainwindow, QString newTuneMame, QString newDivision, int newLHch, int newRHch ) {

    QSettings conf = QSettings( QDir::homePath() + ConfigFileName, QSettings::NativeFormat);
    conf.beginGroup( "TUNES" );
    QStringList data = conf.value( mainwindow->curTuneName ).toStringList();
        data[1] = newDivision;
        data[2] = QString::number( newLHch );
        data[3] = QString::number( newRHch );
    conf.remove( mainwindow->curTuneName );
    conf.setValue( newTuneMame, data );

    mainwindow->curTuneName = newTuneMame;
    left_hand_channel = newLHch;
    right_hand_channel = newRHch;
    mainwindow->setWindowTitle( newTuneMame );
}

void deleteTune( MainWindow *mainwindow ) {
    QSettings conf = QSettings( QDir::homePath() + ConfigFileName, QSettings::NativeFormat);
    conf.beginGroup( "TUNES" );
    QStringList data = conf.value( mainwindow->curTuneName ).toStringList();

    if( data.size() > 4 ) {
        QFile::remove( QDir::homePath() + ConfigTuneDirectory + data[4]);
        conf.remove( mainwindow->curTuneName );
    }

    mainwindow->curTuneName = QString();
    mainwindow->setWindowTitle( QString() );
}

void deletePart( MainWindow *mainwindow, QString part ) {
    if( mainwindow->curTuneName.isEmpty() ) return;
    QString fileName;
    {
        QSettings conf = QSettings( QDir::homePath() + ConfigFileName, QSettings::NativeFormat);
        conf.beginGroup( "TUNES" );
        fileName = QDir::homePath() + ConfigTuneDirectory + conf.value( mainwindow->curTuneName ).toStringList()[4];
    }
    QSettings conf = QSettings( fileName, QSettings::NativeFormat);
    conf.beginGroup( "PARTS" );
    conf.remove( part );
}