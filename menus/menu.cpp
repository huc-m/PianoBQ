#include "mainwindow.h"

#include "midi/midi_with_fluidsynth.h"

void MainWindow::createMenus() {
    fileMenu = menuBar()->addMenu( "File" );
        fileMenu->addAction( tuneNewAction );
        fileMenu->addAction( tuneDivisionsAction );
        fileMenu->addSeparator();
        fileMenu->addAction( exitProgramAction );
    tuneMenu = menuBar()->addMenu( "Tune" );
        tuneMenu->addAction( tuneOpenAction );
        tuneMenu->addAction( tuneRehearsalGetAction);
        tuneMenu->addSeparator();
        tuneMenu->addAction( tuneRehearsalSaveAction );
        tuneMenu->addAction( tuneChangeConfigAction );
    handMenu = menuBar()->addMenu( "Hands" );
        handMenu->addAction( handAllHandsAction );
        handMenu->addAction( handNoHandsAction );
        handMenu->addSeparator();
        handMenu->addAction( handLeftOnlyAction );
        handMenu->addAction( handRightOnlyAction );
        handMenu->addAction( handWithSoundAction );
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
    exitProgramAction = new QAction( "Exit", this );
        connect( exitProgramAction, &QAction::triggered, this, &MainWindow::exitProgram );

    tuneOpenAction = new QAction( "Open", this );
        connect( tuneOpenAction, &QAction::triggered, this, &MainWindow::open_tuneOpenDialog );
    tuneChangeConfigAction = new QAction( "Change conf", this );
        connect( tuneChangeConfigAction, &QAction::triggered, this, &MainWindow::open_tuneChangeConfigDialog );
    tuneRehearsalGetAction = new QAction( "Get Part", this );
        connect( tuneRehearsalGetAction, &QAction::triggered, this, &MainWindow::open_tuneRehearsalGetDialog );
    tuneRehearsalSaveAction = new QAction( "Save Part", this );
        connect( tuneRehearsalSaveAction, &QAction::triggered, this, &MainWindow::open_tuneRehearsalSaveDialog );
    tuneNewAction = new QAction( "New", this );
        connect( tuneNewAction, &QAction::triggered, this, &MainWindow::open_tuneNewDialog );
    tuneDivisionsAction = new QAction( "Divisions", this );
        connect( tuneDivisionsAction, &QAction::triggered, this, &MainWindow::open_tuneDivisionsDialog );

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

    handAllHandsAction = new QAction( "Check All", this );
        connect( handAllHandsAction, &QAction::triggered, this, &MainWindow::handAllHands );
    handNoHandsAction = new QAction( "Free" );
        connect( handNoHandsAction, &QAction::triggered, this, &MainWindow::handNoHands );
    handLeftOnlyAction = new QAction( "Left Only" );
        connect( handLeftOnlyAction, &QAction::triggered, this, &MainWindow::handLeftOnly );
    handRightOnlyAction = new QAction( "Right Only" );
        connect( handRightOnlyAction, &QAction::triggered, this, &MainWindow::handRightOnly );
    handWithSoundAction = new QAction( "With Sound" );
        connect( handWithSoundAction, &QAction::triggered, this, &MainWindow::handWithSound );

        handWithSoundAction->setCheckable( true ); handWithSoundAction->setChecked( false );
    handGroup = new QActionGroup( this );
        handAllHandsAction->setCheckable( true ); handGroup->addAction( handAllHandsAction );
        handNoHandsAction->setCheckable( true ); handGroup->addAction( handNoHandsAction );
        handLeftOnlyAction->setCheckable( true ); handGroup->addAction( handLeftOnlyAction );
        handRightOnlyAction->setCheckable( true ); handGroup->addAction( handRightOnlyAction );
        handGroup->setExclusive( true );

    tunePlayAction = new QAction( "Play" , this );
    connect(menuBar(), &QMenuBar::triggered, this, &MainWindow::actionTriggered);
}

void MainWindow::actionTriggered(QAction *action){
    extern int hand;
    if( action->text() == "Play" ) {
        handWithSoundAction->setChecked( false );
        handWithSound();
        if( this->windowTitle().isEmpty() ) return;
        fluid_play();
    } else
    if( tunePlayAction->text() == "Stop" ) {
        fluid_play( false );
        set_hand( hand );
    }
}