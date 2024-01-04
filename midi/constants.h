#ifndef GLOBAL_CONSTANTS_H
#define GLOBAL_CONSTANTS_H

#define NULL_FILE_NAME "/dev/null"
#define TUNE_LENGTH_MAX 5000

#define LE_H 0  // left hand
#define RI_H 1  // right hand
//#define NOTES8    // if definet max notes = 8 else 4

//error on read mmidi file MIDI_Fluidsynth::read_midi_file
#define MIDI_FILE_TOO_MANY_NOTES_IN_TUPLE 3
#define MIDI_FILE_TUNE_IS_TOO_LONG 2
#define MIDI_FILE_ERROR_ON_READING 1

//MIDI status byte
#define MIDI_NOTE_ON 0x90
#define MIDI_NOTE_OFF 0x80
#endif // GLOBAL_CONSTANTS_H
