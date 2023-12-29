#ifndef GLOBALS_H
#define GLOBALS_H

#include <cstdint>
#include "constants.h"

#ifdef NOTES8
#define Nmax 8
union tune_def {
    unsigned __int128  all; // both hands
    union {
        uint64_t hand;      // one hand
        uint8_t note[8];    // indibidual notes
    } hand[2];
};
#else
#define Nmax 4
union tune_def {
    uint64_t all;          // both hands
    union {
        uint32_t hand;      // one hand
        uint8_t note[4];    // individual notes
    } hand[2];
};
#endif
extern tune_def tune_data[TUNE_LENGTH_MAX];
extern int tune_length;

extern int left_hand_channel;
extern int right_hand_channel;


#endif // GLOBALS_H
