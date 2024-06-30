#ifndef TUNEDIVISIONSDIALOG_H
#define TUNEDIVISIONSDIALOG_H

#include <QDialog>

namespace Ui {
class tuneDivisionsDialog;
}

class tuneDivisionsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit tuneDivisionsDialog(QWidget *parent = nullptr);
    ~tuneDivisionsDialog();

   void accept() override;

private:
    Ui::tuneDivisionsDialog *ui;

public slots:
    void buttonRemoveClicked(bool checked);

};

#endif // TUNEDIVISIONSDIALOG_H
