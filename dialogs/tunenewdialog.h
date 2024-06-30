#ifndef TUNENEWDIALOG_H
#define TUNENEWDIALOG_H

#include <QDialog>

namespace Ui {
class tuneNewDialog;
}

class tuneNewDialog : public QDialog
{
    Q_OBJECT

public:
    explicit tuneNewDialog(QWidget *parent = nullptr);
    ~tuneNewDialog();

private slots:
    void on_pushButton_released();
    void on_buttonBox_accepted();

private:
    Ui::tuneNewDialog *ui;

    QString fileName;

};

#endif // TUNENEWDIALOG_H
