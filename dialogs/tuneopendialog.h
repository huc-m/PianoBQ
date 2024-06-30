#ifndef TUNEOPENDIALOG_H
#define TUNEOPENDIALOG_H

#include <QDialog>

namespace Ui {
class tuneOpenDialog;
}

class tuneOpenDialog : public QDialog
{
    Q_OBJECT

public:
    explicit tuneOpenDialog(QWidget *parent = nullptr);
    ~tuneOpenDialog();

    void accept() override;

private:
    Ui::tuneOpenDialog *ui;

private slots:
    void synchroList(QString division );
};

#endif // TUNEOPENDIALOG_H
