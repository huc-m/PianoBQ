#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "midi/midi_with_fluidsynth.h"

void MainWindow::createMenus() {
    menuBar()->addSeparator();
        menuBar()->addAction( tuneToBeginAction );
        menuBar()->addAction( tuneMoveLeftManyAction );
        menuBar()->addAction( tuneMoveLeftOneAction );
        menuBar()->addAction( tuneMoveRightOneAction );
        menuBar()->addAction( tuneMoveRightManyAction );
    menuBar()->addSeparator();
        menuBar()->addAction( tuneDelStartAction );
        menuBar()->addAction( tuneSetStartAction );
        menuBar()->addAction( tuneSetFinishAction );
        menuBar()->addAction( tuneDelFinishAction );
    menuBar()->addSeparator();
        menuBar()->addAction( tunePlayAction );
}

void MainWindow::createActions() {
    connect( ui->tuneNewAction, &QAction::triggered, this, &MainWindow::open_tuneNewDialog );
    connect( ui->tuneDivisionsAction, &QAction::triggered, this, &MainWindow::open_tuneDivisionsDialog );
    connect( ui->loadFontAction, &QAction::triggered, this, &MainWindow::loadFont );
    connect( ui->exitProgramAction, &QAction::triggered, this, &MainWindow::exitProgram );

    connect( ui->tuneOpenAction, &QAction::triggered, this, &MainWindow::open_tuneOpenDialog );
    connect( ui->tuneRehearsalGetAction, &QAction::triggered, this, &MainWindow::open_tuneRehearsalGetDialog );
    connect( ui->tuneRehearsalSaveAction, &QAction::triggered, this, &MainWindow::open_tuneRehearsalSaveDialog );
    connect( ui->tuneChangeConfigAction, &QAction::triggered, this, &MainWindow::open_tuneChangeConfigDialog );

    connect( ui->handAllHandsAction, &QAction::triggered, this, &MainWindow::handAllHands );
    connect( ui->handNoHandsAction, &QAction::triggered, this, &MainWindow::handNoHands );
    connect( ui->handLeftOnlyAction, &QAction::triggered, this, &MainWindow::handLeftOnly );
    connect( ui->handRightOnlyAction, &QAction::triggered, this, &MainWindow::handRightOnly );
    connect( ui->handWithSoundAction, &QAction::triggered, this, &MainWindow::handWithSound );

    handGroup = new QActionGroup( this );
        handGroup->addAction( ui->handAllHandsAction );
        handGroup->addAction( ui->handNoHandsAction );
        handGroup->addAction( ui->handLeftOnlyAction );
        handGroup->addAction( ui->handRightOnlyAction );
    handGroup->setExclusive( true );

    tuneToBeginAction = new QAction( "!<", this );
        connect( tuneToBeginAction, &QAction::triggered, this, &MainWindow::tuneToBegin );
    tuneMoveLeftOneAction = new QAction( " <", this);
        connect( tuneMoveLeftOneAction, &QAction::triggered, this, &MainWindow::tuneMoveLeftOne );
    tuneMoveLeftManyAction = new QAction( "<<", this);
        connect( tuneMoveLeftManyAction, &QAction::triggered, this, &MainWindow::tuneMoveLeftMany );
    tuneMoveRightManyAction = new QAction( ">>", this);
        connect( tuneMoveRightManyAction, &QAction::triggered, this, &MainWindow::tuneMoveRightMany );
    tuneMoveRightOneAction = new QAction( "> ", this);
        connect( tuneMoveRightOneAction, &QAction::triggered, this, &MainWindow::tuneMoveRightOne );
    tuneDelStartAction = new QAction( "<!", this );
        connect( tuneDelStartAction, &QAction::triggered, this, &MainWindow::tuneDelStart );
    tuneSetStartAction = new QAction( "!=", this );
        connect( tuneSetStartAction, &QAction::triggered, this, &MainWindow::tuneSetStart );
    tuneSetFinishAction = new QAction( "=!", this );
        connect( tuneSetFinishAction, &QAction::triggered, this, &MainWindow::tuneSetFinish );
    tuneDelFinishAction = new QAction( "!>", this );
        connect( tuneDelFinishAction, &QAction::triggered, this, &MainWindow::tuneDelFinish );

    tunePlayAction = new QAction( "Play" , this );
    connect(menuBar(), &QMenuBar::triggered, this, &MainWindow::actionTriggered);
}

void MainWindow::actionTriggered(QAction *action){
    extern int hand;
    if( action->text() == "Play" ) {
        ui->handWithSoundAction->setChecked( false );
        handWithSound();
        if( this->windowTitle().isEmpty() ) return;
        fluid_play();
    } else
    if( tunePlayAction->text() == "Stop" ) {
        fluid_play( false );
        set_hand( hand );
    }
}

void MainWindow::loadFont() {
    QFontDatabase::removeAllApplicationFonts();
    if( ui->loadFontAction->isChecked() ) QFontDatabase::addApplicationFont( QDir::homePath() + FONT_FILE_WITH_LETTER );
    else QFontDatabase::addApplicationFont( QDir::homePath() + FONT_FILE_NO_LETTER );
    begin = -1; staff_area_size = QSize(10,10);
    mainwindow->update();
}