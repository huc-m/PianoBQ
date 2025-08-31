#include "tunechangeconfigdialog.h"
#include "ui_tunechangeconfigdialog.h"

#include "configuration/tuneconfig.h"
#include "configuration/divisions.h"
#include "configuration/edittuneconfig.h"
#include "mainwindow.h"
#include "dialogs/tuneopendialog.h"

#include "midi/midi_with_fluidsynth.h"
#include "midi/globals.h"

tuneChangeConfigDialog::tuneChangeConfigDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::tuneChangeConfigDialog)
{
    setAttribute(Qt::WA_DeleteOnClose);
    ui->setupUi(this);

    ui->buttonBoxParts->addButton( ui->pushButtonDelDiv, QDialogButtonBox::ActionRole );
    ui->buttonBoxFile->addButton( ui->pushButtonDelFiile, QDialogButtonBox::ActionRole );
    ui->buttonBoxStaff->addButton( ui->pushButtonStaff, QDialogButtonBox::ActionRole );

    connect( ui->buttonBoxFile, &QDialogButtonBox::clicked, this, &tuneChangeConfigDialog::fileAction );
    connect( ui->buttonBoxParts, &QDialogButtonBox::clicked, this, &tuneChangeConfigDialog::partsAction);

    ui->lineEditTuneName->setText( mainwindow->windowTitle() );
    ui->spinBoxLHch->setValue( left_hand_channel );
    ui->spinBoxRHch->setValue( right_hand_channel );
    ui->comboBoxDiv->addItems( readDivisions() );
    ui->comboBoxDiv->setCurrentText( getDivisionByTuneName() );

    ui->listWidgetParts->addItems(  getPartsByTune() );

    ui->spinBoxStaffTop->setValue( ((MainWindow*)this->parent())->staff_pading_h );
    ui->spinBoxStaffStep->setValue( ((MainWindow*)this->parent())->staff_step );

    connect( ui->buttonBoxStaff, &QDialogButtonBox::clicked, this, &tuneChangeConfigDialog::staffAction );

    setFixedSize( size() );
}

tuneChangeConfigDialog::~tuneChangeConfigDialog()
{
    delete ui;
}


void tuneChangeConfigDialog::fileAction( QAbstractButton *button) {
    if( button->text() == "Close" ) { close(); return; }


    if( button->text() == "Apply" ) {
        editTuneConfig( ui->lineEditTuneName->text() , ui->comboBoxDiv->currentText(), ui->spinBoxLHch->value(), ui->spinBoxRHch->value() );
        mainwindow->cur_devision_pos = ui->comboBoxDiv->currentIndex();

        read_midi_file( getTuneFile().toStdString().c_str() );
        mainwindow->begin = -1;
        mainwindow->update();
    } else
        if( button->text() == "Delete" ){
            deleteTune();

            tune_length = 0;
            reset_keyboard_fluid( -1 );
            mainwindow->begin = -1;
            mainwindow->update();
            close();
    }
    static_cast <tuneOpenDialog*> (mainwindow->tuneopendialog)->refreshDivisions();
}

void tuneChangeConfigDialog::partsAction( QAbstractButton *button ) {
    if( button->text() == "Close" ) { close(); return; }
    if( button->text() == "Delete" ) {
        if( ui->listWidgetParts->currentRow() < 0 ) return;
        deletePart( ui->listWidgetParts->currentItem()->text() );
        close();
    }
}

void tuneChangeConfigDialog::staffAction( QAbstractButton *button ){
    if( button->text() == "Close" ) { close(); return; }
    if( button->text() == "Apply" ) {
        mainwindow->staff_pading_h = ui->spinBoxStaffTop->value();
        mainwindow->staff_step = ui->spinBoxStaffStep->value();
        mainwindow->begin = -1;
        mainwindow->setStaffParameters();
        mainwindow->update();
        saveStaffForm();
    }
    if( button->text() == "Default" ) {
        ui->spinBoxStaffTop->setValue( mainwindow->staff_pading_h_default );
        ui->spinBoxStaffStep->setValue( mainwindow->staff_step_default );
    }
};