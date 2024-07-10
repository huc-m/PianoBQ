#ifndef GLOBALS_H
#define GLOBALS_H

#include <cstdint>
#include "constants.h"

#ifdef NOTES8
#define N_MAX 8
union tune_def {
    unsigned __int128  all; // both hands
    uint8_t noteOn[16];
    union {
        uint64_t hand;      // one hand
        uint8_t note[8];    // indibidual notes
    } hand[2];
};
#else
#define N_MAX 4
union tune_def {
    uint64_t all;          // both hands
    uint8_t noteOn[8];
    union {
        uint32_t hand;      // one hand
        uint8_t note[4];    // individual notes
    } hand[2];
};
#endif

extern tune_def tune_data[TUNE_LENGTH_MAX]; //all notes
extern tune_def tune_newNotes[TUNE_LENGTH_MAX]; // only new notes
extern tune_def tune_draw[TUNE_LENGTH_MAX]; //to draw
extern uint8_t tuple_nums[TUNE_LENGTH_MAX][TUPLE_NUMS_LENGTH]; // for each hand and all
extern int tune_length;
extern int cur_position;
extern int cur_start;
extern int cur_finish;

extern int midiTicks[TUNE_LENGTH_MAX];

extern int left_hand_channel;
extern int right_hand_channel;

#endif // GLOBALS_H
