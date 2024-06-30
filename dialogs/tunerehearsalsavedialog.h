#ifndef TUNEREHEARSALSAVEDIALOG_H
#define TUNEREHEARSALSAVEDIALOG_H

#include <QDialog>

namespace Ui {
class tuneRehearsalSaveDialog;
}

class tuneRehearsalSaveDialog : public QDialog
{
    Q_OBJECT

public:
    explicit tuneRehearsalSaveDialog(QWidget *parent = nullptr);
    ~tuneRehearsalSaveDialog();

    void accept() override;

private:
    Ui::tuneRehearsalSaveDialog *ui;
};

#endif // TUNEREHEARSALSAVEDIALOG_H
