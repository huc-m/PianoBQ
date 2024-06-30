QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    configuration/edittuneconfig.cpp \
    configuration/fluidsettings.cpp \
    configuration/mainconfig.cpp \
    configuration/tuneconfig.cpp \
    dialogs/tunechangeconfigdialog.cpp \
    dialogs/tunedivisionsdialog.cpp \
    dialogs/tunenewdialog.cpp \
    dialogs/tuneopendialog.cpp \
    dialogs/tunerehearsalgetdialog.cpp \
    dialogs/tunerehearsalsavedialog.cpp \
    drawstatff.cpp \
    main.cpp \
    mainwindow.cpp \
    menus/menu.cpp \
    menus/menu_dialogs.cpp \
    menus/menu_tune.cpp \
    midi/midi_keyboard_libfluidsynth.cpp \
    midi/read_midi_file_fluidsynth.cpp

HEADERS += \
	configuration/configurationconstant.h \
	configuration/edittuneconfig.h \
	configuration/mainconfig.h \
	configuration/tuneconfig.h \
	dialogs/tunechangeconfigdialog.h \
	dialogs/tunedivisionsdialog.h \
	dialogs/tunenewdialog.h \
	dialogs/tuneopendialog.h \
	dialogs/tunerehearsalgetdialog.h \
	dialogs/tunerehearsalsavedialog.h \
	mainwindow.h \
	midi/globals.h \
	midi/constants.h \
	midi/midi_with_fluidsynth.h

FORMS += \
    dialogs/tunechangeconfigdialog.ui \
    dialogs/tunedivisionsdialog.ui \
    dialogs/tunenewdialog.ui \
    dialogs/tuneopendialog.ui \
    dialogs/tunerehearsalgetdialog.ui \
    dialogs/tunerehearsalsavedialog.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

unix:!macx: LIBS += -lfluidsynth

DISTFILES += \
    LICENSE \
    README.md \
    fonts/PianoBQ.sfd \
    fonts/PianoBQ.ttf
