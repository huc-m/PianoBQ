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
    ui->lineEdit->setFocus();
}

tuneRehearsalSaveDialog::~tuneRehearsalSaveDialog()
{
    delete ui;
}

void tuneRehearsalSaveDialog::accept(){
    saveTunePart( (MainWindow*)this->parent(), ui->lineEdit->text() );

    QDialog::accept();
}