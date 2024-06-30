#include <fluidsynth.h>

#include "configuration/mainconfig.h"
#include "configuration/configurationconstant.h"

#include <QSettings>


void FluidSettingsKeyboard(fluid_settings_t *fluid_settings ) {
    QSettings conf = QSettings( QDir::homePath() + FluidFileName, QSettings::NativeFormat);
    conf.beginGroup( "INTEGERS" );
        for( QString key : conf.allKeys() ) fluid_settings_setint( fluid_settings, key.toStdString().c_str(), conf.value( key ).toInt() );
    conf.endGroup();
    conf.beginGroup( "NUMBERS" );
        for( QString key : conf.allKeys() ) fluid_settings_setnum( fluid_settings, key.toStdString().c_str(), conf.value( key ).toDouble() );
    conf.endGroup();
    conf.beginGroup( "STRINGS" );
        for( QString key : conf.allKeys() ) fluid_settings_setstr( fluid_settings, key.toStdString().c_str(), conf.value( key ).toString().toStdString().c_str() );
    conf.endGroup();
}