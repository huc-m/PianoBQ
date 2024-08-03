#ifndef MIDI_WITH_FLUIDSYNTH_H
#define MIDI_WITH_FLUIDSYNTH_H

#include "midi/globals.h"

int read_midi_file(const char *midi_file_name);

void init_keyboard_libfluidsynth(QSettings *conf);
void reset_keyboard_fluid(int cur_pos ); // < 0 full reset
void delete_keyboard_fluid();
void fluid_play( bool play = true, bool del = true );
void read_keyboard_with_sound_start();

void set_hand( int key );
void oneHandStartPos();
void oneHandMoveNextMrg();
void oneHandMovePreviousMrg();

void continueOneHandWithSound();

#endif // MIDI_WITH_FLUIDSYNTH_H
