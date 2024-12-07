#include "fingeringdialog.h"
#include "ui_fingeringdialog.h"

#include "mainwindow.h"
#include "midi/globals.h"


fingeringDialog::fingeringDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::fingeringDialog)
{
    ui->setupUi(this);
    connect( ui->pushButton_toRight, &QPushButton::clicked, this, &fingeringDialog::right );
    connect( ui->pushButton_toLeft, &QPushButton::clicked, this, &fingeringDialog::left );
}

fingeringDialog::~fingeringDialog()
{
    delete ui;
}

void fingeringDialog::left(){
    saveData();
    mainwindow->tuneMoveLeftOne();
    getData();
}

void fingeringDialog::right(){
    saveData();
    mainwindow->tuneMoveRightOne();
    getData();
}

void fingeringDialog::saveData(){
    mainwindow->fingering[cur_position][LE_H] = *(int32_t*)( ui->lineEdit_Left->text().toStdString().c_str() );
    mainwindow->fingering[cur_position][RI_H] = *(int32_t*)( ui->lineEdit_Right->text().toStdString().c_str() );
    mainwindow->begin = -1; mainwindow->update();
}

void fingeringDialog::getData(){
    QString fng;
    (fng = QString((char*) &mainwindow->fingering[cur_position][RI_H])).truncate(4);
      ui->lineEdit_Right->setText( fng );
    (fng = QString((char*) &mainwindow->fingering[cur_position][LE_H])).truncate(4);
       ui->lineEdit_Left->setText( fng );
}

void fingeringDialog::open(){
    mainwindow->fingeringLoad();
    getData();

    QDialog::open();
}

void fingeringDialog::accept(){
    saveData();
    mainwindow->fingeringSave();

    QDialog::accept();
}

void fingeringDialog::reject(){
    saveData();

    QDialog::reject();
}