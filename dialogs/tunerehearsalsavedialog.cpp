#include "tunerehearsalsavedialog.h"
#include "ui_tunerehearsalsavedialog.h"

#include "configuration/tuneconfig.h"
#include "mainwindow.h"

#include <QLineEdit>


tuneRehearsalSaveDialog::tuneRehearsalSaveDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::tuneRehearsalSaveDialog)
{
    setAttribute(Qt::WA_DeleteOnClose);
    ui->setupUi(this);
    ui->comboBox->addItems( getPartsByTune() );

    setFixedSize( size() );

    connect( ui->buttonGroupNum, &QButtonGroup::buttonClicked, this, &tuneRehearsalSaveDialog::typeNumber );

}

tuneRehearsalSaveDialog::~tuneRehearsalSaveDialog()
{
    delete ui;
}

void tuneRehearsalSaveDialog::accept(){
    if(  ui->comboBox->currentText().length() < 1 ) return;
    QString part = ui->comboBox->currentText();
    saveTunePart( part );
    mainwindow->setToolboxParts();
    tuneAcceptPart( part );

    QDialog::accept();
}

void tuneRehearsalSaveDialog::typeNumber( QAbstractButton *numClick ) {
    if( numClick->text() == "<" ) ui->comboBox->lineEdit()->backspace();
    else ui->comboBox->lineEdit()->insert( numClick->text() );
}