#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "midi/globals.h"
#include "midi/midi_with_fluidsynth.h"

void MainWindow::handAllHands(){
    ui->handAllHandsAction->setChecked( true );
    ui->buttonGroupHands->button(-4)->setChecked( true );
    set_hand( ALL_H );
}

void MainWindow::handNoHands(){
    ui->buttonGroupHands->button(-5)->setChecked( true );
    set_hand( NO_H );
}

void MainWindow::handLeftOnly(){
    ui->handLeftOnlyAction->setChecked( true );
    ui->buttonGroupHands->button(-2)->setChecked( true );
    if( ui->handWithSoundAction->isChecked() ) set_hand( LE_D );
    else set_hand( LE_H );
}

void MainWindow::handRightOnly(){
    ui->handRightOnlyAction->setChecked( true );
    ui->buttonGroupHands->button(-3)->setChecked( true );
    if( ui->handWithSoundAction->isChecked() ) set_hand( RI_D );
    else set_hand( RI_H );
}

void MainWindow::handWithSound(){
    if( !ui->handWithSoundAction->isChecked() ) fluid_play( false );
    if( ui->handLeftOnlyAction->isChecked() ) { handLeftOnly(); return; }
    if( ui->handRightOnlyAction->isChecked() ) { handRightOnly(); return; }
}