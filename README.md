### PianoBQ
Piano teacher for amateurs

### Introduction
PianoBQ (Beginer Qt version) checks that the pianist is pressing only the correct keys in a sequence specified in a MIDI file.

### Functionality
- graphics view of staff with note leters
- separating left and right hand notes based on different channels
- step and page scrolling back and forth
- rehearsal for left and right hand only
- library of tunes grouped by category
- dynamically defining the beginning and end of a part of a tune being played
- storing the defined parts of each tune for repeated rehearsal
- play current midi file

### Install
There is a binary provided that runs on Debian 12 ("bookworm"). To run a binary, you need to save the configuration files in ~/.config/PianoBQ/. It should also be ensured that the relevant libraries are present on the system (such as libfluidsynth and the applicable QT libraries).
