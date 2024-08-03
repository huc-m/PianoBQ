#ifndef TUNECONFIG_H
#define TUNECONFIG_H

#include <QString>
#include <QStringList>

void init_tune_conf();

void saveTune( QString fileName, QString division );
QStringList getTunesByDivision( QString division );
QString getTuneFile();
QStringList getPartsByTune();
void setStartFinishByPart(QString partName );
void saveTunePart(QString partName );

void saveStaffForm();

#endif // TUNECONFIG_H
