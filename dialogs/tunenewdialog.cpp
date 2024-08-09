#include "dialogs/tunenewdialog.h"
#include "ui_tunenewdialog.h"

#include "configuration/divisions.h"
#include "configuration/tuneconfig.h"

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
    ui->spinBoxLH->setValue( mainwindow->left_hand_channel_default );
    ui->spinBoxRH->setValue( mainwindow->right_hand_channel_default );

    setFixedSize( size() );
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
    mainwindow->currentPath = fileInfo.absoluteDir().absolutePath();
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
    } else {
        mainwindow->setWindowTitle( ui->tuneName->text() );
        left_hand_channel = ui->spinBoxLH->value();
        right_hand_channel = ui->spinBoxRH->value();

        saveTune( ui->fileName->text(), ui->comboBox->currentText() );
        init_tune_conf();
    }
    mainwindow->begin = -1;
    mainwindow->update();
}
