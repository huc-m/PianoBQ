#include "fingeringdialog.h"
#include "ui_fingeringdialog.h"

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "midi/globals.h"

fingeringDialog::fingeringDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::fingeringDialog)
{
    ui->setupUi(this);
    connect( ui->pushButton_toRight, &QPushButton::clicked, this, &fingeringDialog::right );
    connect( ui->pushButton_toLeft, &QPushButton::clicked, this, &fingeringDialog::left );

    setFixedSize( size() );
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
    std::string fng;

    fng.assign ( ui->lineEdit_Right->text().toUtf8().data() );
    std::sort(fng.begin(), fng.end(), std::greater<char>() );
    strncpy( (char*)( &mainwindow->fingering[cur_position][RI_H] ), fng.data(), 4 );

    fng.assign ( ui->lineEdit_Left->text().toUtf8().data() );
    std::sort( fng.begin(), fng.end(), std::less<char>() );
    strncpy( (char*)( &mainwindow->fingering[cur_position][LE_H] ), fng.data(), 4 );

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

    freeHand( true );

    QDialog::open();
}

void fingeringDialog::accept(){
    saveData();
    mainwindow->fingeringSave();

    freeHand( false );

    QDialog::accept();
}

void fingeringDialog::reject(){
    saveData();

    freeHand( false );

    QDialog::reject();
}

void fingeringDialog::freeHand( bool set ){
    if( set ){
        handsMenu = mainwindow->handGroup->checkedAction();
        mainwindow->ui->handNoHandsAction->activate( QAction::Trigger );
    } else handsMenu->activate( QAction::Trigger );
}