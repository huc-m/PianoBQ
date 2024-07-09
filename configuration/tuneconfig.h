#ifndef TUNECONFIG_H
#define TUNECONFIG_H

#include <QString>
#include <QStringList>

#include "mainwindow.h"

void saveTune( MainWindow *mainwindow, QString fileName, QString division );
QStringList getTunesByDivision( QString division );
QString getTuneFile(MainWindow *mainwindow );
QStringList getPartsByTune(MainWindow *mainwindow);
void setStartFinishByPart( MainWindow *mainwindow, QString partName );
void saveTunePart(MainWindow *mainwindow, QString partName );

bool setStaffForm(MainWindow *mainwindow);
void saveStaffForm(MainWindow *mainwindow);

#endif // TUNECONFIG_H
