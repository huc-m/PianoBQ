#ifndef TUNEREHEARSALGETDIALOG_H
#define TUNEREHEARSALGETDIALOG_H

#include <QDialog>

namespace Ui {
class tuneRehearsalGetDialog;
}

class tuneRehearsalGetDialog : public QDialog
{
    Q_OBJECT

public:
    explicit tuneRehearsalGetDialog(QWidget *parent = nullptr);
    ~tuneRehearsalGetDialog();

    void accept() override;

private:
    Ui::tuneRehearsalGetDialog *ui;
};

#endif // TUNEREHEARSALGETDIALOG_H
