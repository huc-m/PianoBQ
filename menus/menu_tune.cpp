#include "mainwindow.h"

#include "midi/globals.h"
#include "midi/midi_with_fluidsynth.h"
extern int hand;

void MainWindow::tuneMoveLeftOne() {
    if( --cur_position < cur_start ) cur_position = cur_start;
    if( hand == LE_H || hand == RI_H ) {
        oneHandMovePreviousMrg();
        reset_keyboard_fluid( cur_position );
    }
    if( hand == LE_D || hand == RI_D ) set_hand( hand );
    update();
}

void MainWindow::tuneMoveLeftMany(){
    if( ( cur_position -= numVisibleNotes ) < cur_start ) cur_position = cur_start;
    if( hand == LE_H || hand == RI_H ) {
        oneHandMovePreviousMrg();
        reset_keyboard_fluid( cur_position );
    }
    if( hand == LE_D || hand == RI_D ) set_hand( hand );
    update();
}

void MainWindow::tuneMoveRightOne() {
    if( ++cur_position > cur_finish ) cur_position = cur_finish;
    if( hand == LE_H || hand == RI_H ) {
        oneHandMoveNextMrg();
        reset_keyboard_fluid( cur_position );
    }
    if( hand == LE_D || hand == RI_D ) set_hand( hand );
    update();
}

void MainWindow::tuneMoveRightMany(){
    if( ( cur_position += numVisibleNotes ) > cur_finish ) cur_position = cur_finish;
    if( hand == LE_H || hand == RI_H ) {
        oneHandMoveNextMrg();
        reset_keyboard_fluid( cur_position );
    }
    if( hand == LE_D || hand == RI_D ) set_hand( hand );
    update();
}

void MainWindow::tuneToBegin() {
    reset_keyboard_fluid( cur_start );
    if( hand == LE_H || hand == RI_H ) {
        oneHandMoveNextMrg();
        reset_keyboard_fluid( cur_position );
    }
    if( hand == LE_D || hand == RI_D ) set_hand( hand );
    update();
}

void MainWindow::tuneSetStart() {
    cur_start = cur_position;
    reset_keyboard_fluid( cur_position );
    update();
}

void MainWindow::tuneSetFinish() {
    cur_finish = cur_position;
    reset_keyboard_fluid( cur_position );
}

void MainWindow::tuneDelStart() {
    cur_start = 0;
    reset_keyboard_fluid( cur_position );
}

void MainWindow::tuneDelFinish() {
    cur_finish = tune_length;
}