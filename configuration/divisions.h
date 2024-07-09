#ifndef DIVISIONS_H
#define DIVISIONS_H

#include <QString>
#include <QStringList>

#include "mainwindow.h"

QStringList readDivisions();
QStringList readNotUsedDivisions();
void saveDivisions( QStringList dlist );
void delDivision( QString division );

QString getDivisionByTuneName( MainWindow *mainwindow );

#endif // DIVISIONS_H
