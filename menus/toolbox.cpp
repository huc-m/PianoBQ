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
    tuneAcceptPart(mainwindow->ui->comboBox_Part->currentText() );

};

void MainWindow::setToolboxParts() {
    if( mainwindow->tunePlayAction->text() == "Stop" ) fluid_play( false );
    ui->comboBox_Part->clear();
    ui->comboBox_Part->addItems( getPartsByTune() );
    ui->comboBox_Part->setCurrentIndex( -1 );
}

void MainWindow::setToolbox() {
    connect( ui->pushButton_Open, &QPushButton::clicked, this, toolBox_openPart );
    connect( ui->comboBox_Part, &QComboBox::currentTextChanged, this, toolBox_AcceptParts );
}


