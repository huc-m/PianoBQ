#include "tuneopendialog.h"
#include "ui_tuneopendialog.h"

#include "configuration/tuneconfig.h"
#include "midi/midi_with_fluidsynth.h"
#include "midi/globals.h"

tuneOpenDialog::tuneOpenDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::tuneOpenDialog)
{
    setAttribute(Qt::WA_DeleteOnClose);
    ui->setupUi(this);

    ui->comboBox->addItems( readDivisions() );
    connect( ui->comboBox, &QComboBox::currentTextChanged, this, &tuneOpenDialog::synchroList );
    ui->comboBox->setCurrentIndex( ((MainWindow*)this->parent())->cur_devision_pos );
    if( ui->comboBox->currentIndex() == 0 ) emit ui->comboBox->currentTextChanged( ui->comboBox->currentText() );

    connect( ui->listWidget, &QListWidget::doubleClicked, this, &tuneOpenDialog::accept );
}

tuneOpenDialog::~tuneOpenDialog()
{
    delete ui;
}

void tuneOpenDialog::synchroList(QString division) {
    ((MainWindow*)this->parent())->cur_devision_pos = ui->comboBox->currentIndex();
    ui->listWidget->clear();
    ui->listWidget->addItems( getTunesByDivision( division ));
}

void tuneOpenDialog::accept(){
    if( ui->listWidget->currentRow() >= 0 ){
        ((MainWindow*)this->parent())->curTuneName = ui->listWidget->currentItem()->text();
            ((MainWindow*)this->parent())->setWindowTitle( ((MainWindow*)this->parent())->curTuneName );
            ((MainWindow*)this->parent())->cur_devision_pos = ui->comboBox->currentIndex();

        read_midi_file( getTuneFile( (MainWindow*)this->parent() ).toStdString().c_str() );
        reset_keyboard_fluid( -1 );
        ((MainWindow*)this->parent())->cur_pos = cur_start;
        cur_finish = tune_length;
        ((MainWindow*)this->parent())->begin = -1;
        ((MainWindow*)this->parent())->update();
    }
    QDialog::accept();
}