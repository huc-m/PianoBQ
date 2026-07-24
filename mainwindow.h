#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#define CONFIG_MAIN_FILE QDir::homePath() + "/.config/PianoBQ/main.conf"
#define CONFIG_TUNES_FILE QDir::homePath() + "/.config/PianoBQ/tunes.conf"
#define CONFIG_TUNES_DIRECTORY QDir::homePath() + "/.config/PianoBQ/tunes/"

#ifndef DEB_PACK
    #define FONT_FILE_WITH_LETTER QDir::homePath() + "/.config/PianoBQ/fonts/PianoBQ.ttf"
    #define FONT_FILE_NO_LETTER QDir::homePath() + "/.config/PianoBQ/fonts/PianoBQn.ttf"
#else
    #define FONT_FILE_WITH_LETTER "/usr/share/PianoBQ/fonts/PianoBQ.ttf"
    #define FONT_FILE_NO_LETTER "/usr/share/PianoBQ/fonts/PianoBQn.ttf"
    #define CONFIG_MAIN_FILE_DEFAULT "/usr/share/PianoBQ/main.conf"
#endif

#define G5S 80
#define D4F 62
#define B3F 59
#define F2F 41

#define ALERT_NOTE 90 //#F6 1480Hz

#include <QMainWindow>
#include <QMenuBar>
#include <QDialog>
#include <QGraphicsScene>

#include <QActionGroup>

#include <QTimer>

#include "myqgraphicsview.h"

#include "dialogs/tuneopendialog.h"
#include "dialogs/tunenewdialog.h"
#include "dialogs/fingeringdialog.h"

#include "midi/constants.h"

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
friend void tuneAcceptPart( QString part );
friend void toolBox_changePlaySpeed();
friend void toolBox_openFingeringDialog();
friend tuneOpenDialog;
friend tuneNewDialog;

friend void toolBox_openPart();
friend void toolBox_AcceptParts();
friend void toolBox_setWholeTune();
friend void toolBox_Hands( int );

friend void toolBox_Time();

friend fingeringDialog;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QTimer *timer = new QTimer(this);
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
    QFont nFont, fFont;

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
    int numUpcomingNotes; //number of notes at the end before the screen scrolls

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
    QActionGroup *handGroup;

    QAction *tuneToBeginAction;
    QAction *tuneMoveLeftOneAction;
    QAction *tuneMoveLeftManyAction;
    QAction *tuneMoveRightOneAction;
    QAction *tuneMoveRightManyAction;
    QAction *tuneToEndAction;

    QAction *tuneSetStartAction;
    QAction *tuneSetFinishAction;
    QAction *tuneDelStartAction;
    QAction *tuneDelFinishAction;

    QAction * tunePlayAction;

    void createMenus();
    void createActions();


    QDialog *tunenewdialog;
    QDialog *tunedivisionsdialog;
    QDialog *tunerehearsalsavedialog;
    QDialog *tunerehearsalgetdialog;
    QDialog *tunechangeconfigdialog;

public:
    QDialog *tuneopendialog;
    QDialog *fingeringdialog;


    void setToolbox();
    void setToolboxParts();

private slots:
// File
    void exitProgram();
//Tune
    void open_tuneOpenDialog();
    void open_tuneNewDialog();
    void open_tuneDivisionsDialog();
    void loadFont();
    void showPartOnlySwitch();
    void progresBarShowSwitch();
    void fingeringShowSwitch();
    void open_tuneRehearsalSaveDialog();
    void open_tuneRehearsalGetDialog();
    void open_tuneChangeConfigDialog();
    void open_fingeringDialog();

    void tuneToBegin();
    void tuneToEnd();

    void tuneMoveLeftOne();
    void tuneMoveLeftMany();
    void tuneMoveRightOne();
    void tuneMoveRightMany();
    void tuneSetStart();
    void tuneSetFinish();
    void tuneDelStart();
    void tuneDelFinish();

    void actionTriggered( QAction *action );

public:
    void handAllHands();
    void handNoHands();
    void handLeftOnly();
    void handRightOnly();
    void handWithSound();

public: // globals
    QString currentPath;
    int cur_devision_pos;
    int begin;         //begin position on the staff in the graphics
    bool showPartOnly;
    bool progresBarShow;
    bool fingeringShow;
    bool notDigitalPiano;
    int alertNoteVelocity;

// fingering
public:
    int32_t fingering[TUNE_LENGTH_MAX][2];
    bool fingering_isLoaded;

    void fingeringLoad();
    void fingeringSave();

};

extern MainWindow *mainwindow;

#endif // MAINWINDOW_H
