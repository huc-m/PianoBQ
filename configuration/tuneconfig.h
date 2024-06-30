#ifndef TUNECONFIG_H
#define TUNECONFIG_H

#include <QString>
#include <QStringList>

#include "mainwindow.h"

QStringList readDivisions();
QStringList readNotUsedDivisions();
void saveDivisions( QStringList dlist );
void delDivision( QString division );

void saveTune( MainWindow *mainwindow, QString fileName, QString division );
QStringList getTunesByDivision( QString division );
QString getTuneFile(MainWindow *mainwindow );
QStringList getPartsByTune(MainWindow *mainwindow);
void setStartFinishByPart( MainWindow *mainwindow, QString partName );
void saveTunePart(MainWindow *mainwindow, QString partName );

QString getDivisionByTuneName( MainWindow *mainwindow );

#endif // TUNECONFIG_H
