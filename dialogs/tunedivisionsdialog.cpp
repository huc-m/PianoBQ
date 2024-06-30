#include "tunedivisionsdialog.h"
#include "ui_tunedivisionsdialog.h"

#include "mainwindow.h"
#include "configuration/tuneconfig.h"

tuneDivisionsDialog::tuneDivisionsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::tuneDivisionsDialog)
{
    setAttribute(Qt::WA_DeleteOnClose);
    ui->setupUi(this);

    ui->buttonBoxDel->addButton(ui->pushButtonRemove, QDialogButtonBox::ActionRole);

    ui->comboBox->addItems( readDivisions() );
    ui->listWidget->addItems( readNotUsedDivisions() );

    connect( ui->buttonBoxAdd, &QDialogButtonBox::rejected, this, &tuneDivisionsDialog::reject );
    connect( ui->buttonBoxDel, &QDialogButtonBox::rejected, this, &tuneDivisionsDialog::reject );
    connect( ui->buttonBoxAdd, &QDialogButtonBox::accepted, this, &tuneDivisionsDialog::accept );
    connect( ui->pushButtonRemove, &QPushButton::clicked, this, &tuneDivisionsDialog::buttonRemoveClicked );

}

tuneDivisionsDialog::~tuneDivisionsDialog()
{
    delete ui;
}


void tuneDivisionsDialog::buttonRemoveClicked( bool checked ) {
    if( ui->listWidget->currentRow() < 0 ) return;
    delDivision( ui->listWidget->currentItem()->text() );
    close();
}

void tuneDivisionsDialog::accept() {
    if(  ui->comboBox->currentText().length() < 1 ) return;
    if ( ui->comboBox->findText(ui->comboBox->currentText(),Qt::MatchExactly) >= 0 ) return;

    ui->comboBox->addItem( ui->comboBox->currentText(),"");
    QStringList divisions;
    for( int i = 0; i < ui->comboBox->count(); ++i) divisions.append(ui->comboBox->itemText( i ));
    saveDivisions( divisions );

    QDialog::accept();
}