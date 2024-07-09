#include "configuration/mainconfig.h"
#include "configuration/configurationconstant.h"

#include "midi/globals.h"

#include <QSettings>

void setMidiChannels() {
    QSettings conf = QSettings( QDir::homePath() + MainConfigFileName, QSettings::NativeFormat);
    conf.beginGroup( "MIDI_CHANNELS" );
        left_hand_channel =  conf.value( "LeftHandChannel" ).toInt();
        right_hand_channel = conf.value( "RightHandChannel" ).toInt();
}

void setSoudFont( MainWindow *mainwindow ) {
    QSettings conf = QSettings( QDir::homePath() + MainConfigFileName, QSettings::NativeFormat);
    conf.beginGroup( "SOUND_FONT" );
        mainwindow->sf2FileName = conf.value( "file" ).toString();
}

void setParameters( MainWindow *mainwindow ) {
    QSettings conf = QSettings( QDir::homePath() + MainConfigFileName, QSettings::NativeFormat);
    conf.beginGroup( "PARAMETERS" );
        mainwindow->currentPath = conf.value( "currentPath" ).toString();
    conf.endGroup();
    conf.beginGroup( "STAFF" );
        mainwindow->staff_base_h2 = conf.value( "half_space_between_lines" ).toString().toInt();
        mainwindow->staff_step_default = conf.value( "staffs_step" ).toString().toInt();
        mainwindow->staff_pading_h_default = conf.value( "padding_top" ).toString().toInt();
        mainwindow->staff_pading_w = conf.value( "padding_left" ).toString().toInt();
        mainwindow->staff_font_z = conf.value( "font_size" ).toString().toInt();
        mainwindow->staff_line_w = conf.value( "line_width" ).toString().toInt();
}