#include "tuneopendialog.h"
#include "ui_tuneopendialog.h"

#include "configuration/divisions.h"
#include "configuration/tuneconfig.h"
#include "mainwindow.h"

#include "ui_mainwindow.h"

#include "midi/midi_with_fluidsynth.h"
#include "midi/globals.h"
#include "midi/constants.h"

tuneOpenDialog::tuneOpenDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::tuneOpenDialog)
{
    ui->setupUi(this);

    ui->comboBox->addItems( readDivisions() );
    connect( ui->comboBox, &QComboBox::currentTextChanged, this, &tuneOpenDialog::synchroList );
    ui->comboBox->setCurrentIndex( mainwindow->cur_devision_pos );
    if( ui->comboBox->currentIndex() == 0 ) emit ui->comboBox->currentTextChanged( ui->comboBox->currentText() );

    connect( ui->listWidget, &QListWidget::doubleClicked, this, &tuneOpenDialog::accept );

    setFixedSize( size() );
}

tuneOpenDialog::~tuneOpenDialog()
{
    delete ui;
}

void tuneOpenDialog::synchroList(QString division) {
    ui->listWidget->clear();
    ui->listWidget->addItems( getTunesByDivision( division ));
}

void tuneOpenDialog::accept(){
    if( ui->listWidget->currentRow() >= 0 ){
        mainwindow->setWindowTitle( ui->listWidget->currentItem()->text() );
            mainwindow->cur_devision_pos = ui->comboBox->currentIndex();
        init_tune_conf();
        mainwindow->setToolboxParts();

        read_midi_file( getTuneFile().toStdString().c_str() );
        reset_keyboard_fluid( -1 );
        mainwindow->begin = -1;
        mainwindow->update();
    }
    mainwindow->ui->comboBox_Speed->setCurrentText( "1.0" );
    mainwindow->ui->progressBar->setMaximum( tune_length );
    mainwindow->ui->progressBar->setValue( 0 );
    QDialog::accept();
}

void tuneOpenDialog::refreshDivisions(){
    ui->comboBox->clear();
    ui->comboBox->addItems( readDivisions() );
    ui->comboBox->setCurrentIndex( mainwindow->cur_devision_pos );
    emit ui->comboBox->currentTextChanged( ui->comboBox->currentText() );
}