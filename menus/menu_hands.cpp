#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "midi/globals.h"
#include "midi/midi_with_fluidsynth.h"

void MainWindow::handAllHands(){
    set_hand( ALL_H );
}

void MainWindow::handNoHands(){
    set_hand( NO_H );
}

void MainWindow::handLeftOnly(){
    if( ui->handWithSoundAction->isChecked() ) set_hand( LE_D );
    else set_hand( LE_H );
}

void MainWindow::handRightOnly(){
    if( ui->handWithSoundAction->isChecked() ) set_hand( RI_D );
    else set_hand( RI_H );
}

void MainWindow::handWithSound(){
    if( !ui->handWithSoundAction->isChecked() ) fluid_play( false );
    if( ui->handLeftOnlyAction->isChecked() ) { handLeftOnly(); return; }
    if( ui->handRightOnlyAction->isChecked() ) { handRightOnly(); return; }
}