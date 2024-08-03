#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#define CONFIG_MAIN_FILE "/.config/PianoBQ/main.conf"
#define CONFIG_TUNES_FILE "/.config/PianoBQ/tunes.conf"
#define CONFIG_TUNES_DIRECTORY "/.config/PianoBQ/tunes/"
#define FONT_FILE "/.config/PianoBQ/fonts/PianoBQ.ttf"

#define G5S 80
#define D4F 62
#define B3F 59
#define F2F 41

#include <QMainWindow>
#include <QMenuBar>
#include <QDialog>
#include <QGraphicsScene>

#include <QActionGroup>

#include "myqgraphicsview.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

friend myQGraphicsView;
friend void init_tune_conf();
friend void fluid_play( bool, bool );
friend int play_update_graphics ( void*, fluid_midi_event_t* );

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QPixmap *staffPixmap;
    QGraphicsPixmapItem *staffPixmapItem;
    QPainter *paint;
    void DrawStaff(int);

    void setStaffParameters();

    int staff_step_default;
    int staff_pading_h_default;
    int left_hand_channel_default;
    int right_hand_channel_default;

    int staff_step;
    int staff_pading_h;
    int staff_pading_w;
    int staff_line_w;  //width of line
    int staff_font_z;  //size of font
    int note_zero[2];  //line of the note with code 0 - left and right

protected:

    void resizeEvent(QResizeEvent* event) override;
    void paintEvent(QPaintEvent *event) override;

private:
    Ui::MainWindow *ui;


    QGraphicsScene staffScene;
    QSize staff_area_size;

    void DrawTuple(int pos_screen, int pos_tune);
    int staff_base_h;   //space between lines
    int staff_base_h2;  //half
    int staff_base_h7;  //*7
    int staff_base_w;   //space between tuples

    int numVisibleNotes;  //number of notes that are visible on one staff screen

// offset C, #C, D, #D, E, F, #F, G, #G, A, #A, B
    const int note_ofset[12] = {0,0,1,1,2,3,3,4,4,5,5,6};
    const char noteC_symbol = 'A';
    const char ledjer_symbol = '@';
    const int other_note_offset = 24;
    const int ledjer_offset[24] = {0,0, 12,12, 0, 12,12, 0,0, 12,12, 0,   12,12, 0,0, 12, 0,0, 12,12, 0,0, 12};
    const int ledjer0[24] = {1,1, 2,2, 1, 2,2, 1,1, 2,2, 1,   2,2, 1,1, 2, 1,1, 2,2, 1,1, 2};
    const int staff_low[2] = {F2F, D4F}; //low notes in the staff for both hands
    const int staff_high[2] = {B3F, G5S};

private:
    QMenu *fileMenu;
    QMenu *tuneMenu;
    QMenu *handMenu;

    QAction *exitProgramAction;

    QAction *tuneOpenAction;
    QAction *tuneToBeginAction;
    QAction *tuneNewAction;
    QAction *tuneDivisionsAction;
    QAction *tuneChangeConfigAction;

    QAction *tuneMoveLeftOneAction;
    QAction *tuneMoveLeftManyAction;
    QAction *tuneMoveRightOneAction;
    QAction *tuneMoveRightManyAction;
    QAction *tuneSetStartAction;
    QAction *tuneSetFinishAction;
    QAction *tuneDelStartAction;
    QAction *tuneDelFinishAction;
    QAction *tuneRehearsalSaveAction;
    QAction *tuneRehearsalGetAction;

    QAction *handAllHandsAction;
    QAction *handNoHandsAction;
    QAction *handLeftOnlyAction;
    QAction *handRightOnlyAction;
    QAction *handWithSoundAction;
    QActionGroup *handGroup;

    QAction * tunePlayAction;

    void createMenus();
    void createActions();

    QDialog *tuneopendialog;
    QDialog *tunenewdialog;
    QDialog *tunedivisionsdialog;
    QDialog *tunerehearsalsavedialog;
    QDialog *tunerehearsalgetdialog;
    QDialog *tunechangeconfigdialog;

private slots:
// File
    void exitProgram();
//Tune
    void open_tuneOpenDialog();
    void open_tuneNewDialog();
    void open_tuneDivisionsDialog();
    void open_tuneRehearsalSaveDialog();
    void open_tuneRehearsalGetDialog();
    void open_tuneChangeConfigDialog();

    void tuneToBegin();

    void tuneMoveLeftOne();
    void tuneMoveLeftMany();
    void tuneMoveRightOne();
    void tuneMoveRightMany();
    void tuneSetStart();
    void tuneSetFinish();
    void tuneDelStart();
    void tuneDelFinish();

    void handAllHands();
    void handNoHands();
    void handLeftOnly();
    void handRightOnly();
    void handWithSound();

    void actionTriggered( QAction *action );

public: // globals
    QString currentPath;
    int cur_devision_pos;
    int begin;         //begin position on the staff in the graphics
};

#endif // MAINWINDOW_H
