#include "tunerehearsalgetdialog.h"
#include "ui_tunerehearsalgetdialog.h"

#include "configuration/tuneconfig.h"
#include "mainwindow.h"
#include "midi/globals.h"
#include "midi/midi_with_fluidsynth.h"

tuneRehearsalGetDialog::tuneRehearsalGetDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::tuneRehearsalGetDialog)
{
    setAttribute(Qt::WA_DeleteOnClose);
    ui->setupUi(this);
    ui->listWidget->addItems( getPartsByTune( (MainWindow*)(this->parent()) ) );
    connect( ui->listWidget, &QListWidget::doubleClicked, this, &tuneRehearsalGetDialog::accept );
}

tuneRehearsalGetDialog::~tuneRehearsalGetDialog()
{
    delete ui;
}

void tuneRehearsalGetDialog::accept() {
    if( ui->listWidget->currentRow() >= 0 ) {
        setStartFinishByPart( (MainWindow*)(this->parent()), ui->listWidget->currentItem()->text() );
        reset_keyboard_fluid( ((MainWindow*)this->parent())->cur_pos = cur_start );
        ((MainWindow*)this->parent())->startOneHand();
        ((MainWindow*)this->parent())->update();
    }
    QDialog::accept();
}