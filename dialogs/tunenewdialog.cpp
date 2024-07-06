#include "dialogs/tunenewdialog.h"
#include "ui_tunenewdialog.h"

#include "configuration/tuneconfig.h"
#include "configuration/mainconfig.h"

#include <QFileDialog>
#include <QMessageBox>

#include "mainwindow.h"
#include "midi/globals.h"
#include "midi/midi_with_fluidsynth.h"

tuneNewDialog::tuneNewDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::tuneNewDialog)
{
    setAttribute(Qt::WA_DeleteOnClose);
    ui->setupUi(this);
    ui->comboBox->addItems( readDivisions() );
    setMidiChannels();
    ui->spinBoxLH->setValue( left_hand_channel );
    ui->spinBoxRH->setValue( right_hand_channel );
}

tuneNewDialog::~tuneNewDialog()
{
    delete ui;
}

void tuneNewDialog::on_pushButton_released(){
    fileName = QFileDialog::getOpenFileName( this, "Select midi file", ((MainWindow*)this->parent())->currentPath, "Midi Files (*.mid)");
    ui->fileName->setText(fileName);
    QFileInfo fileInfo = QFileInfo( fileName );
    ui->tuneName->setText( fileInfo.baseName());
    ((MainWindow*)this->parent())->currentPath = fileInfo.absoluteDir().absolutePath();
}


void tuneNewDialog::on_buttonBox_accepted(){
    int err = read_midi_file( fileName.toStdString().c_str() );
    if( err != NO_ERRORS ){
        QMessageBox msgBox;
        switch ( err ) {
            case MIDI_FILE_TOO_MANY_NOTES_IN_TUPLE:
                msgBox.setText("To many notes in hord.");
            break;
            case MIDI_FILE_TUNE_IS_TOO_LONG:
                msgBox.setText("Tune is too long.");
            break;
            case MIDI_FILE_ERROR_ON_READING:
                msgBox.setText("Error in file.");
        }
        msgBox.exec();
        tune_length = 0;
        ((MainWindow*)this->parent())->setWindowTitle( "Failed to read file" );
    } else {
        ((MainWindow*)this->parent())->curTuneName = ui->tuneName->text();
            ((MainWindow*)this->parent())->setWindowTitle( ((MainWindow*)this->parent())->curTuneName );
        left_hand_channel = ui->spinBoxLH->value();
        right_hand_channel = ui->spinBoxRH->value();

        saveTune( (MainWindow*)(this->parent()), ui->fileName->text(), ui->comboBox->currentText() );
    }

    reset_keyboard_fluid( -1 );
    ((MainWindow*)this->parent())->cur_pos = cur_start;
    ((MainWindow*)this->parent())->begin = -1;
    ((MainWindow*)this->parent())->update();
}
