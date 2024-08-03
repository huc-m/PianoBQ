#include "mainwindow.h"

#include "midi/globals.h"
#include "midi/midi_with_fluidsynth.h"

void MainWindow::handAllHands(){
    set_hand( ALL_H );
}

void MainWindow::handNoHands(){
    set_hand( NO_H );
}

void MainWindow::handLeftOnly(){
    if( handWithSoundAction->isChecked() ) set_hand( LE_D );
    else set_hand( LE_H );
}

void MainWindow::handRightOnly(){
    if( handWithSoundAction->isChecked() ) set_hand( RI_D );
    else set_hand( RI_H );
}

void MainWindow::handWithSound(){
    if( !handWithSoundAction->isChecked() ) fluid_play( false );
    if( handLeftOnlyAction->isChecked() ) { handLeftOnly(); return; }
    if( handRightOnlyAction->isChecked() ) { handRightOnly(); return; }
}