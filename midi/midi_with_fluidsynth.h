#ifndef MIDI_WITH_FLUIDSYNTH_H
#define MIDI_WITH_FLUIDSYNTH_H

#include "mainwindow.h"

int read_midi_file(char *midi_file_name);
void read_midi_keyboard_with_libfluidsynth(MainWindow* mainWin);

#endif // MIDI_WITH_FLUIDSYNTH_H
