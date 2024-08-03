#ifndef DIVISIONS_H
#define DIVISIONS_H

#include <QString>
#include <QStringList>

QStringList readDivisions();
QStringList readNotUsedDivisions();
void saveDivisions( QStringList dlist );
void delDivision( QString division );

QString getDivisionByTuneName();

#endif // DIVISIONS_H
