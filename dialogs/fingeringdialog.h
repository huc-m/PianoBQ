#ifndef FINGERINGDIALOG_H
#define FINGERINGDIALOG_H

#include <QDialog>

namespace Ui {
class fingeringDialog;
}

class fingeringDialog : public QDialog
{
    Q_OBJECT

public:
    explicit fingeringDialog(QWidget *parent = nullptr);
    ~fingeringDialog();

    void open() override;

private:
    Ui::fingeringDialog *ui;

    void left();
    void right();
    void saveData();
    void getData();
    void freeHand( bool set );

    void accept() override;
    void reject() override;

    QAction *handsMenu;
};

#endif // FINGERINGDIALOG_H
