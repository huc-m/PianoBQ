#ifndef MIDI_WITH_FLUIDSYNTH_H
#define MIDI_WITH_FLUIDSYNTH_H

#include "mainwindow.h"

int read_midi_file(const char *midi_file_name);
void read_midi_keyboard_with_libfluidsynth(MainWindow* mainWin);
void delete_keyboard_fluid();
void reset_keyboard_fluid( int from_pos ); //from_pos < 0 full reset

#endif // MIDI_WITH_FLUIDSYNTH_H
