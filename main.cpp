#include "mainwindow.h"

#include "midi/globals.h"
#include "midi/midi_with_fluidsynth.h"

#include <QApplication>

int tune_length;
tune_def tune_data[TUNE_LENGTH_MAX];
tune_def tune_newNotes[TUNE_LENGTH_MAX];
tune_def tune_draw[TUNE_LENGTH_MAX];
uint8_t tuple_nums[TUNE_LENGTH_MAX][5];
tune_def tune_pastNotes[TUNE_LENGTH_MAX];
int left_hand_channel;
int right_hand_channel;

const char
    *ConfigTuneDirectory = "/.config/PianoBQ/tunes/",
    *ConfigFileName = "/.config/PianoBQ/tunes.conf",
    *MainConfigFileName = "/.config/PianoBQ/main.conf",
    *FontFile = "/.config/PianoBQ/fonts/PianoBQ.ttf",
    *FluidFileName = "/.config/PianoBQ/fluid.conf";

int main(int argc, char *argv[])
{
    QApplication theApp(argc, argv);
    MainWindow AppWindow;

    int err = -1;

    if(argc == 2) {
        err = read_midi_file(argv[1]);
        if( err != NO_ERRORS ) {
            switch ( err ) {
                case MIDI_FILE_TUNE_IS_TOO_LONG:
                    printf( "\n Tune is too long\n\n");
                break;
                case MIDI_FILE_TOO_MANY_NOTES_IN_TUPLE:
                    printf( " \nToo manu notes in tuple\n\n");
                break;
                default:
                    printf( "\n Error reading midi file\n\n");
            }
        exit( err );
        }
    }

    AppWindow.show();
    AppWindow.cur_pos=0; AppWindow.begin=-1;
    AppWindow.update();

    read_midi_keyboard_with_libfluidsynth( &AppWindow );

    return theApp.exec();
}
