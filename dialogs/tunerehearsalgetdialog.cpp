#include "tunerehearsalgetdialog.h"
#include "ui_tunerehearsalgetdialog.h"

#include "configuration/tuneconfig.h"
#include "mainwindow.h"
#include "midi/globals.h"
#include "midi/midi_with_fluidsynth.h"

extern MainWindow *mainwindow;
extern int hand;

tuneRehearsalGetDialog::tuneRehearsalGetDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::tuneRehearsalGetDialog)
{
    setAttribute(Qt::WA_DeleteOnClose);
    ui->setupUi(this);
    ui->listWidget->addItems( getPartsByTune() );
    connect( ui->listWidget, &QListWidget::doubleClicked, this, &tuneRehearsalGetDialog::accept );

    setFixedSize( size() );
}

tuneRehearsalGetDialog::~tuneRehearsalGetDialog()
{
    delete ui;
}

void tuneRehearsalGetDialog::accept() {
    if( ui->listWidget->currentRow() >= 0 ) {
        setStartFinishByPart(  ui->listWidget->currentItem()->text() );
        reset_keyboard_fluid( cur_start );
        if( hand == LE_D || hand == RI_D ){
            oneHandStartPos();
            reset_keyboard_fluid( cur_position );
        }
        mainwindow->update();
    }
    QDialog::accept();
}