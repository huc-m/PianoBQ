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
    QStringList notUsed = conf.value( "DIVISIONS/divisions" ).toStringList();
    conf.beginGroup( "TUNES" );

    for( QString division : notUsed )
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

void saveTune(MainWindow* mainwindow, QString fileName, QString division){
    QSettings conf = QSettings( QDir::homePath() + ConfigFileName, QSettings::NativeFormat);
    QString fileNumber;
    fileNumber.setNum( conf.value("OTHER/LastFileNumber").toInt() + 1 );
    conf.setValue("OTHER/LastFileNumber", fileNumber);

    conf.beginGroup( "TUNES" );
    conf.setValue(mainwindow->curTuneName, QStringList( { fileName, division, QString::number( left_hand_channel ), QString::number( right_hand_channel ),  fileNumber} ));
}

QStringList getTunesByDivision( QString division ){
    QSettings conf = QSettings( QDir::homePath() + ConfigFileName, QSettings::NativeFormat);
    conf.beginGroup( "TUNES" );
    QStringList list;

    for( QString key : conf.allKeys() ) if( conf.value( key ).toStringList()[1] == division ) list.append( key );

    return list;
}

QString getTuneFile( MainWindow *mainwindow ) {
    QSettings conf = QSettings( QDir::homePath() + ConfigFileName, QSettings::NativeFormat);
    conf.beginGroup( "TUNES" );
    QStringList data = conf.value( mainwindow->curTuneName).toStringList();
        left_hand_channel = data[2].toInt();
        right_hand_channel = data[3].toInt();

    return data[0];
}

void saveTunePart(MainWindow *mainwindow, QString partName) {
    if( mainwindow->curTuneName.isEmpty() ) return;
    QString fileName;
    {
        QSettings conf = QSettings( QDir::homePath() + ConfigFileName, QSettings::NativeFormat);
        conf.beginGroup( "TUNES" );
        fileName = QDir::homePath() + ConfigTuneDirectory + conf.value( mainwindow->curTuneName ).toStringList()[4];
    }

    QStringList list = { QString::number( cur_start ), QString::number( cur_finish )};

    QSettings conf = QSettings( fileName, QSettings::NativeFormat);
    conf.beginGroup( "PARTS" );
    conf.setValue( partName, list );
}

QStringList getPartsByTune( MainWindow *mainwindow ) {
    if( mainwindow->curTuneName.isEmpty() ) return QStringList();
    QSettings conf = QSettings( QDir::homePath() + ConfigFileName, QSettings::NativeFormat);
    conf.beginGroup( "TUNES" );
    QSettings conf1 = QSettings( QDir::homePath() + ConfigTuneDirectory + conf.value( mainwindow->curTuneName ).toStringList()[4], QSettings::NativeFormat);
    conf1.beginGroup( "PARTS" );
    return conf1.allKeys();
}

void setStartFinishByPart( MainWindow *mainwindow, QString partName ) {
    QSettings conf = QSettings( QDir::homePath() + ConfigFileName, QSettings::NativeFormat);
    conf.beginGroup( "TUNES" );
    QSettings conf1 = QSettings( QDir::homePath() + ConfigTuneDirectory + conf.value( mainwindow->curTuneName ).toStringList()[4], QSettings::NativeFormat);
    conf1.beginGroup( "PARTS" );
    QStringList data = conf1.value( partName ).toStringList();
    cur_start = data[0].toInt();
    cur_finish = data[1].toInt();
}

QString getDivisionByTuneName( MainWindow *mainwindow ) {

    QSettings conf = QSettings( QDir::homePath() + ConfigFileName, QSettings::NativeFormat);
    conf.beginGroup( "TUNES" );
    QStringList data = conf.value( mainwindow->curTuneName).toStringList();

    if( data.size() > 1 ) return data[1];
    else return QString();
}

