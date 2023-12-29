#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFontDatabase>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->graphicsView->setScene(&staffScene);

//set parameters

    staff_base_h = 18;
    staff_base_h2 = staff_base_h / 2;
    staff_base_h7 = staff_base_h2 * 7;
    staff_step = 160;
    staff_pading_h = 70;
    staff_pading_w = 30;
    staff_font_z = 52;
    staff_base_w = staff_font_z * 1.5;
    staff_line_w =2;
    note_zero_r = staff_pading_h + staff_base_h2*46;
    note_zero_l = note_zero_r + staff_step - staff_base_h*6;
    staff_font_n = "../fonts/PianoBQ.ttf";

//set staff view

    staffPixmap = new QPixmap(ui->graphicsView->size());
    staffPixmapItem = staffScene.addPixmap(*staffPixmap);
    paint = new QPainter(staffPixmap);
    paint->setPen(QPen(Qt::black, staff_line_w));
    QFontDatabase:: addApplicationFont(staff_font_n);
    paint->setFont(QFont("PianoBQ",staff_font_z));
}

MainWindow::~MainWindow()
{
    paint->end();
    delete staffPixmap;
    delete paint;
    QFontDatabase:: removeAllApplicationFonts();

    delete ui;
}

