#ifndef TUNECHANGECONFIGDIALOG_H
#define TUNECHANGECONFIGDIALOG_H

#include <QDialog>
#include <QAbstractButton>

namespace Ui {
class tuneChangeConfigDialog;
}

class tuneChangeConfigDialog : public QDialog
{
    Q_OBJECT

public:
    explicit tuneChangeConfigDialog(QWidget *parent = nullptr);
    ~tuneChangeConfigDialog();

private:
    Ui::tuneChangeConfigDialog *ui;

public slots:
    void fileAction( QAbstractButton *button );
    void partsAction( QAbstractButton *button );
    void staffAction( QAbstractButton *button );
};

#endif // TUNECHANGECONFIGDIALOG_H
