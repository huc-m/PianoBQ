#include "tunechangeconfigdialog.h"
#include "ui_tunechangeconfigdialog.h"

#include "configuration/tuneconfig.h"
#include "configuration/divisions.h"
#include "configuration/edittuneconfig.h"

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

    ui->lineEditTuneName->setText( ((MainWindow*)this->parent())->curTuneName );
    ui->spinBoxLHch->setValue( left_hand_channel );
    ui->spinBoxRHch->setValue( right_hand_channel );
    ui->comboBoxDiv->addItems( readDivisions() );
    ui->comboBoxDiv->setCurrentText( getDivisionByTuneName ( (MainWindow*)(this->parent() ) ) );

    ui->listWidgetParts->addItems( getPartsByTune( (MainWindow*)(this->parent()) ) );

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
        editTuneConfig( (MainWindow*)(this->parent()), ui->lineEditTuneName->text() , ui->comboBoxDiv->currentText(), ui->spinBoxLHch->value(), ui->spinBoxRHch->value() );

        read_midi_file( getTuneFile( (MainWindow*)this->parent() ).toStdString().c_str() );
        reset_keyboard_fluid( -1 );
        ((MainWindow*)this->parent())->cur_pos = cur_start;
        cur_finish = tune_length;
        ((MainWindow*)this->parent())->begin = -1;
        ((MainWindow*)this->parent())->update();
        return;
    }

    if( button->text() == "Delete" ){
        deleteTune( (MainWindow*)(this->parent()) );

        reset_keyboard_fluid( -1 );
        ((MainWindow*)this->parent())->cur_pos = cur_start = 0;
        cur_finish = tune_length = 0;
        ((MainWindow*)this->parent())->begin = -1;
        ((MainWindow*)this->parent())->update();
        close(); return;
    }
}

void tuneChangeConfigDialog::partsAction( QAbstractButton *button ) {
    if( button->text() == "Close" ) { close(); return; }
    if( button->text() == "Delete" ) {
        if( ui->listWidgetParts->currentRow() < 0 ) return;
        deletePart( (MainWindow*)(this->parent()), ui->listWidgetParts->currentItem()->text() );
        close();
    }
}

void tuneChangeConfigDialog::staffAction( QAbstractButton *button ){
    if( button->text() == "Close" ) { close(); return; }
    if( button->text() == "Apply" ) {
        ((MainWindow*)this->parent())->staff_pading_h = ui->spinBoxStaffTop->value();
        ((MainWindow*)this->parent())->staff_step = ui->spinBoxStaffStep->value();
        ((MainWindow*)this->parent())->begin = -1;
        ((MainWindow*)this->parent())->setStaffParameters();
        ((MainWindow*)this->parent())->update();
        saveStaffForm( ((MainWindow*)this->parent()) );
    }
    if( button->text() == "Default" ) {
        ui->spinBoxStaffTop->setValue( ((MainWindow*)this->parent())->staff_pading_h_default );
        ui->spinBoxStaffStep->setValue( ((MainWindow*)this->parent())->staff_step_default );
    }
};