#ifndef MAINCONFIG_H
#define MAINCONFIG_H

#include <fluidsynth/types.h>
#include "mainwindow.h"

void setMidiChannels();
void setSoudFont( MainWindow *mainwindow );
void setParameters( MainWindow *mainwindow );

void FluidSettingsKeyboard ( fluid_settings_t *fluid_settings );

#endif // MAINCONFIG_H
