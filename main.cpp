#include "mainwindow.h"

#include "midi/globals.h"
#include "midi/midi_with_fluidsynth.h"

#include <QApplication>

tune_def tune_data[TUNE_LENGTH_MAX];
tune_def tune_newNotes[TUNE_LENGTH_MAX];
tune_def tune_draw[TUNE_LENGTH_MAX];
uint8_t tuple_nums[TUNE_LENGTH_MAX][TUPLE_NUMS_LENGTH];
tune_def tune_pastNotes[TUNE_LENGTH_MAX];
int midiTicks[TUNE_LENGTH_MAX];

int tune_length;
int cur_position;
int cur_start;
int cur_finish;
int left_hand_channel;
int right_hand_channel;

int main(int argc, char *argv[])
{
    QApplication theApp(argc, argv);
    MainWindow AppWindow;

    AppWindow.show();
    AppWindow.update();

    return theApp.exec();
}
