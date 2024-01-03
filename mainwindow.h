#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <qgraphicsscene.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QPixmap *staffPixmap;
    QGraphicsPixmapItem *staffPixmapItem;
    QPainter *paint;
    void DrawStaff(int);

private:
    Ui::MainWindow *ui;

    QGraphicsScene staffScene;
    QSize staff_area_size;

    void DrawTuple(int pos_screen, int pos_tune);

    int staff_base_h;   //space between lines
    int staff_base_h2;  //half
    int staff_base_h7;  //*7
    int staff_base_w;   //space between tuples
    int staff_step;
    int staff_pading_h;
    int staff_pading_w;
    int staff_line_w;  //width of line
    int staff_font_z;  //size of font
    int note_zero_r;   //line of note with code 0 - left
    int note_zero_l;   //right
    char* staff_font_n;

    int begin;         //begin position on staff

// offset C, #C, D, #D, E, F, #F, G, #G, A, #A, B
    const int note_ofset[12] = {0,0,1,1,2,3,3,4,4,5,5,6};
    const char noteC_symbol = 'A';
    const char ledjer_symbol = '@';
// const int ledgerOffset = 12;
    const int other_note_offset = 24;
    const int ledjer_offset[24] = {0,0, 12,12, 0, 12,12, 0,0, 12,12, 0,   12,12, 0,0, 12, 0,0, 12,12, 0,0, 12};
    const int ledjer0[24] = {0,0,0,1,0,0,1,0,0,0,1,0,0,1,0,0,1,0,0,0,1,0,0,1};
};
#endif // MAINWINDOW_H
