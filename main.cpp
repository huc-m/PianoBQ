#include "mainwindow.h"

#include <QApplication>
#include <QGraphicsView>

#include<unistd.h> /////////////////////////////////////////////////////////////////////////

#include "midi/midi_with_fluidsynth.h"

#include "midi/globals.h"
int tune_length;
tune_def tune_data[TUNE_LENGTH_MAX];
int left_hand_channel=1;
int right_hand_channel=0;



int main(int argc, char *argv[])
{
    QApplication theApp(argc, argv);
    MainWindow AppWindow;

    int err;
    if(argc == 2) err = read_midi_file(argv[1]);

    AppWindow.show();
    AppWindow.DrawStaff(0);
    read_midi_keyboard_with_libfluidsynth(&AppWindow);

//    for(int i=0; i<tune_length; i++){
//        AppWindow.DrawStaff(i);
//        usleep(500000);
//    }


return theApp.exec();

    printf("\n err = %d;  length = %d\n", err, tune_length);
    for(int i=0; i<tune_length; i++){
        printf("\nleft :");
            for(int j=0; j<Nmax; ++j) printf("%4d", tune_data[i].hand[0].note[j]);
        printf("    right :");
            for(int j=0; j<Nmax; ++j) printf("%4d", tune_data[i].hand[1].note[j]);

    }

     return theApp.exec();
}
