#include "mainwindow.h"

#include "midi/globals.h"
#include "midi/midi_with_fluidsynth.h"

void MainWindow::startOneHand( bool right ){
    if( handLeftOnlyAction->isChecked() ){
        if ( right )
            while ( tuple_nums[cur_position][LE_D] == 0 ) {
                if( cur_position < cur_finish ) ++cur_position;
                else break;
            }
        else
            while ( tuple_nums[cur_position][LE_D] == 0 )
                if( cur_position > cur_start ) --cur_position;
                else break;
        cur_pos = cur_position;
    }
    if( handRightOnlyAction->isChecked()){
        if ( right )
            while ( tuple_nums[cur_position][RI_D] == 0 ) {
                if( cur_position < cur_finish ) ++cur_position;
                else break;
            }
        else
            while ( tuple_nums[cur_position][RI_D] == 0 )
                if( cur_position > cur_start ) --cur_position;
                else break;
        cur_pos = cur_position;
    }
    if( !right ) startOneHand();
}

void MainWindow::tuneMoveLeftOne() {
    if( --cur_pos < cur_start ) cur_pos = cur_start;
    reset_keyboard_fluid( cur_pos );
    startOneHand( false );
    update();
}

void MainWindow::tuneMoveLeftMany(){
    if( ( cur_pos -= numVisibleNotes ) < cur_start ) cur_pos = cur_start;
    reset_keyboard_fluid( cur_pos );
    startOneHand( false );
    update();
}

void MainWindow::tuneMoveRightOne() {
    if( ++cur_pos > cur_finish ) cur_pos = cur_finish;
    reset_keyboard_fluid( cur_pos );
    startOneHand();
    update();
}

void MainWindow::tuneMoveRightMany(){
    if( ( cur_pos += numVisibleNotes ) > cur_finish ) cur_pos = cur_finish;
    reset_keyboard_fluid( cur_pos );
    startOneHand();
    update();
}

void MainWindow::tuneToBegin() {
    reset_keyboard_fluid( cur_pos = cur_start );
    startOneHand( false );
    update();
}

void MainWindow::tuneSetStart() {
    cur_start = cur_pos;
    reset_keyboard_fluid( cur_pos );
    update();
}

void MainWindow::tuneSetFinish() {
    cur_finish = cur_pos;
    reset_keyboard_fluid( cur_pos );
}

void MainWindow::tuneDelStart() {
    cur_start = 0;
    reset_keyboard_fluid( cur_start );
}

void MainWindow::tuneDelFinish() {
    cur_finish = tune_length;
}