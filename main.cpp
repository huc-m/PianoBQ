#include "mainwindow.h"

#include "midi/globals.h"
#include "midi/midi_with_fluidsynth.h"

#include <QApplication>

tune_def tune_data[TUNE_LENGTH_MAX];
tune_def tune_newNotes[TUNE_LENGTH_MAX];
tune_def tune_draw[TUNE_LENGTH_MAX];
uint8_t tuple_nums[TUNE_LENGTH_MAX][TUPLE_NUMS_LENGTH];
tune_def tune_pastNotes[TUNE_LENGTH_MAX];
int midiTicks[TUNE_LENGTH_MAX];

int tune_length;
int cur_position;
int cur_start;
int cur_finish;
int left_hand_channel;
int right_hand_channel;

double play_speed;

int main(int argc, char *argv[])
{
    if( argc > 1 ) { qInfo("Build on " __DATE__" " __TIME__ " Qt" QT_VERSION_STR ); return 0; }

#ifdef DEB_PACK
    if( !QFile::exists(CONFIG_MAIN_FILE)) {
        QDir bdir; QFileInfo finfo( CONFIG_MAIN_FILE );
        bdir.mkpath( finfo.path() );
        QFile::copy( CONFIG_MAIN_FILE_DEFAULT, CONFIG_MAIN_FILE);
        QSettings *tunes_conf;
        tunes_conf = new QSettings( CONFIG_TUNES_FILE, QSettings::NativeFormat );
        QStringList divisionAll = tunes_conf->value( "DIVISIONS/divisions" ).toStringList();
        if( divisionAll.isEmpty() ) {
            divisionAll.append("Default");
            tunes_conf->setValue("DIVISIONS/divisions", divisionAll );
        }
        delete tunes_conf;
    }
#endif

    QApplication theApp(argc, argv);
    MainWindow AppWindow;

    AppWindow.show();
    AppWindow.update();

    return theApp.exec();
}
