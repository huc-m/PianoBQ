#include "midi/constants.h"
#include "midi/midi_with_fluidsynth.h"

#include "configuration/mainconfig.h"
#include "configuration/configurationconstant.h"

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFontDatabase>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->graphicsView->setScene(&staffScene);
    createActions();
    createMenus();

//set parameters
    setParameters( this );
    staff_step = staff_step_default;
    staff_pading_h = staff_pading_h_default;

    setStaffParameters();

    begin = -1;
    staff_area_size = QSize(10,10);
    cur_devision_pos = -1;

    setSoudFont( this );

//set staff view

    staffPixmap = new QPixmap( staff_area_size );
    staffPixmapItem = staffScene.addPixmap( *staffPixmap );
    paint = new QPainter( staffPixmap );
    QFontDatabase:: addApplicationFont( QDir::homePath() + FontFile );
}

MainWindow::~MainWindow()
{
    delete_keyboard_fluid();

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