#ifndef EDITTUNECONFIG_H
#define EDITTUNECONFIG_H

#include <mainwindow.h>


void editTuneConfig(MainWindow* mainwindow, QString newTuneMame, QString newDivision, int newLHch, int newRHch  );
void deleteTune( MainWindow *mainwindow );
void deletePart( MainWindow *mainwindow, QString part);

#endif // EDITTUNECONFIG_H
