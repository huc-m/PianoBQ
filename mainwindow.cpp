#include "midi/globals.h"
#include "midi/midi_with_fluidsynth.h"

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFontDatabase>
#include <QSettings>

#include "dialogs/tuneopendialog.h"

MainWindow *mainwindow;
QSettings *tune_conf = NULL;
QSettings *tunes_conf;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->graphicsView->setScene(&staffScene);
    mainwindow = this;
    setWindowTitle( NULL );
    tunes_conf = new QSettings( QDir::homePath() + CONFIG_TUNES_FILE, QSettings::NativeFormat );
    createActions();
    createMenus();
    setToolbox();

//set parameters
    QSettings *conf = new QSettings( QDir::homePath() + CONFIG_MAIN_FILE, QSettings::NativeFormat);

    conf->beginGroup( "MIDI" );
        left_hand_channel_default =  conf->value( "LeftHandChannel" ).toInt();
        right_hand_channel_default = conf->value( "RightHandChannel" ).toInt();
        currentPath = conf->value( "currentPath" ).toString();
    conf->endGroup();
    conf->beginGroup( "STAFF" );
        staff_base_h2 = conf->value( "half_space_between_lines" ).toString().toInt();
        staff_step_default = conf->value( "staffs_step" ).toString().toInt();
        staff_pading_h_default = conf->value( "padding_top" ).toString().toInt();
        staff_pading_w = conf->value( "padding_left" ).toString().toInt();
        staff_font_z = conf->value( "font_size" ).toString().toInt();
        staff_line_w = conf->value( "line_width" ).toString().toInt();
        ui->loadFontAction->setChecked( conf->value( "note_with_letter" ).toBool() );
        ui->showPartOnlyAction->setChecked( conf->value( "show_part_only" ).toBool() );
        ui->progressBarAction->setChecked( conf->value( "progressBar_Show" ).toBool());
    conf->endGroup();

    staff_step = staff_step_default;
    staff_pading_h = staff_pading_h_default;

    setStaffParameters();

    staff_area_size = QSize(10,10);
    cur_devision_pos = -1;

    init_keyboard_libfluidsynth( conf );

    delete( conf );

    set_hand( NO_H );

    play_speed = 1;
    showPartOnlySwitch();
    progresBarShowSwitch();

//set staff view

    staffPixmap = new QPixmap( staff_area_size );
    staffPixmapItem = staffScene.addPixmap( *staffPixmap );
    paint = new QPainter( staffPixmap );
    loadFont();

//init dialogs

    tuneopendialog = new tuneOpenDialog( this );
    fingeringdialog = new fingeringDialog( this );
}

MainWindow::~MainWindow()
{
    delete_keyboard_fluid();

    delete tune_conf;
    delete tunes_conf;

    delete tuneopendialog;
    delete fingeringdialog;

    paint->end();
    delete staffPixmap;
    delete paint;
    QFontDatabase:: removeAllApplicationFonts();

    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent* event)
{
   begin = -1;
   update();

   QMainWindow::resizeEvent(event);
}

void MainWindow::setStaffParameters(){

    staff_base_h = staff_base_h2 * 2;
    staff_base_h7 = staff_base_h2 * 7;
    staff_base_w = staff_font_z * 1.3;
    note_zero[RI_H] = staff_pading_h + staff_base_h2*46;
    note_zero[LE_H] = note_zero[RI_H] + staff_step - staff_base_h*6;
}

void MainWindow::fingeringLoad(){
    if( fingering_isLoaded ) return;
    if( tune_conf == NULL ) return;

    QFile *file = new QFile( QDir::homePath() + FINGERINGS_DIRECTORY + QFileInfo( tune_conf->fileName()).baseName() + ".fng");
    if( file->exists()){
        file->open( QIODeviceBase::ReadOnly );
          memcpy( fingering, qUncompress( file->readAll() ).data(), tune_length * 8);
        file->close();
        QByteArray data;
    } else
    for(int i = 0; i < tune_length; ++i )  *(int64_t*)(mainwindow->fingering[i]) = 0;
    fingering_isLoaded = true;
}

void MainWindow::fingeringSave(){
    if( tune_conf == NULL ) return;
    QFile *file = new QFile( QDir::homePath() + FINGERINGS_DIRECTORY + QFileInfo( tune_conf->fileName()).baseName() + ".fng");
    file->open( QIODeviceBase::WriteOnly );
      file->write( qCompress( (uchar*) fingering, tune_length * 8, 9 ));
    file->close();
}