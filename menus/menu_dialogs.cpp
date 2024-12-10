#include "mainwindow.h"
#include "dialogs/tunenewdialog.h"
#include "dialogs/tunedivisionsdialog.h"
#include "dialogs/tuneopendialog.h"
#include "dialogs/tunerehearsalsavedialog.h"
#include "dialogs/tunerehearsalgetdialog.h"
#include "dialogs/tunechangeconfigdialog.h"

#include "midi/globals.h"

void MainWindow::exitProgram() {
    exit( 0 );
}

void MainWindow::open_tuneNewDialog() {
    tunenewdialog = new tuneNewDialog( this );
    tunenewdialog->open();
}

void MainWindow::open_tuneDivisionsDialog() {
    tunedivisionsdialog = new tuneDivisionsDialog( this );
    tunedivisionsdialog->open();
}

void MainWindow::open_tuneOpenDialog() {
    tuneopendialog->open();
}

void MainWindow::open_tuneRehearsalSaveDialog() {
    tunerehearsalsavedialog = new tuneRehearsalSaveDialog( this );
    tunerehearsalsavedialog->open();
}

void MainWindow::open_tuneRehearsalGetDialog() {
    tunerehearsalgetdialog = new tuneRehearsalGetDialog( this );
    tunerehearsalgetdialog->open();
}

void MainWindow::open_tuneChangeConfigDialog() {
    tunechangeconfigdialog = new tuneChangeConfigDialog( this );
    tunechangeconfigdialog->open();
}

void MainWindow::open_fingeringDialog(){
    fingeringdialog->open();
}