#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "configuration/tuneconfig.h"
#include "midi/midi_with_fluidsynth.h"

void toolBox_openPart(){
    if( mainwindow->tunePlayAction->text() == "Stop" ) fluid_play( false );
    mainwindow->open_tuneOpenDialog();
}

void toolBox_AcceptParts() {
    if( mainwindow->tunePlayAction->text() == "Stop" ) fluid_play( false );
    if( mainwindow->ui->comboBox_Part->currentIndex() != -1 )
        tuneAcceptPart(mainwindow->ui->comboBox_Part->currentText() );

};

void toolBox_changePlaySpeed(){
    play_speed = mainwindow->ui->comboBox_Speed->currentText().toDouble();
    if( mainwindow->tunePlayAction->text() == "Stop" ) fluid_play(true);
}

void MainWindow::setToolboxParts() {
    if( mainwindow->tunePlayAction->text() == "Stop" ) fluid_play( false );
    ui->comboBox_Part->clear();
    ui->comboBox_Part->addItems( getPartsByTune() );
    ui->comboBox_Part->setCurrentIndex( -1 );
}

void toolBox_setWholeTune() {
    mainwindow->tuneDelStart();
    mainwindow->tuneDelFinish();
    mainwindow->begin=-1;
}

void toolBox_openFingeringDialog(){
    if( mainwindow->tunePlayAction->text() == "Stop" ) fluid_play( false );
    mainwindow->open_fingeringDialog();
}

void toolBox_Hands( int id ){
    if( mainwindow->tunePlayAction->text() == "Stop" ) fluid_play( false );
    switch ( id ) {
        case -2:
            mainwindow->handLeftOnly();
        break;
        case -3:
            mainwindow->handRightOnly();
        break;
        case -4:
            mainwindow->handAllHands();
        break;

    }
}

void toolBox_Time(){
    mainwindow->ui->ctime->setText(QTime::currentTime().toString("hh:mm"));
}

void MainWindow::setToolbox() {
    connect( ui->pushButton_Open, &QPushButton::clicked, this, toolBox_openPart );
    connect( ui->comboBox_Part, &QComboBox::currentTextChanged, this, toolBox_AcceptParts );
    connect( ui->comboBox_Speed, &QComboBox::currentTextChanged, this, toolBox_changePlaySpeed );
    connect( ui->pushButtonAll, &QPushButton::clicked, this, toolBox_setWholeTune );
    connect( ui->pushButton_OpenFingeringDialog, &QPushButton::clicked, this, toolBox_openFingeringDialog );

    connect( ui->buttonGroupHands, &QButtonGroup::idClicked, this, toolBox_Hands );

    if(!ui->ctime->isHidden()){
        connect( timer, &QTimer::timeout, this, toolBox_Time );
        toolBox_Time();
        timer->start(60000);
    }
}

