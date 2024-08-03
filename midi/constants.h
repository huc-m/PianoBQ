#ifndef CONSTANTS_H
#define CONSTANTS_H

#define NULL_FILE_NAME "/dev/null"
#define SYNTH_CHANNEL_OUT 0

#define TUNE_LENGTH_MAX 10000

#define LE_H 0  // left hand   => don't change
#define RI_H 1  // right hand  => don't change
#define NEW_N 2
#define LE_D 3
#define RI_D 4  // = LE_D + 1
#define TUPLE_NUMS_LENGTH 5 // = number of items above

#define ALL_H 5 // <> *_H
#define NO_H 6  // <> *_H
#define HtoD 3  // = LE_D - LE_H

//#define NOTES8    // if defined max notes per hand = 8 else 4

// error codes
#define NO_ERRORS 0
//error on read midi file MIDI_Fluidsynth::read_midi_file
#define MIDI_FILE_TOO_MANY_NOTES_IN_TUPLE 3
#define MIDI_FILE_TUNE_IS_TOO_LONG 2
#define MIDI_FILE_ERROR_ON_READING 1

//MIDI status byte
#define MIDI_NOTE_ON 0x90
#define MIDI_NOTE_OFF 0x80

#endif // CONSTANTS_H
