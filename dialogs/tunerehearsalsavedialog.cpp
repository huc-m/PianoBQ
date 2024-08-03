#include "tunerehearsalsavedialog.h"
#include "ui_tunerehearsalsavedialog.h"

#include "configuration/tuneconfig.h"
#include "mainwindow.h"

tuneRehearsalSaveDialog::tuneRehearsalSaveDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::tuneRehearsalSaveDialog)
{
    setAttribute(Qt::WA_DeleteOnClose);
    ui->setupUi(this);
    ui->comboBox->addItems( getPartsByTune() );

    setFixedSize( size() );
}

tuneRehearsalSaveDialog::~tuneRehearsalSaveDialog()
{
    delete ui;
}

void tuneRehearsalSaveDialog::accept(){
    if(  ui->comboBox->currentText().length() < 1 ) return;
    saveTunePart( ui->comboBox->currentText() );

    QDialog::accept();
}