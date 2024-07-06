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
    set_hand( LE_H );
}

void MainWindow::handRightOnly(){
    set_hand( RI_H);
}