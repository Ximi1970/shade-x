#
#   Get the defaults
#
include( ../Shade-X.pri )

#
#   Defines
#

#
# Set the Qt modules
#
QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

#
# Define the target
#
TARGET = Shade-X
TEMPLATE = app

#
#  Translations
#
TRANSLATIONS = \
    languages/$${TARGET}.en-US.ts

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

#
#   System dependency
#
win32: {
#    QMAKE_LFLAGS += -static -lwinpthread -static-libgcc -static-libstdc++ $$(QMAKE_LFLAGS_WINDOWS)

    #
    #	Windows host (not used in cross compiling with mingw on Linux)
    #
    !mingw:contains(QMAKE_HOST.os, Windows): {
        LIBS += User32.lib
        LIBS += Comctl32.lib
        LIBS += Dwmapi.lib
    }

    delDir($${PWD}\\..\\dist\\bin)
    mkDir($${PWD}\\..\\dist\\bin)
    copyToDestdir($${DESTDIR}\\$${TARGET}.exe, $${PWD}\\..\\dist\\bin\\$${TARGET}.exe)
    QMAKE_POST_LINK += $$[QT_INSTALL_BINS]\windeployqt.exe $$quote($$shell_path($${PWD}\\..\\dist\\bin\\$${TARGET}.exe)) $$escape_expand(\\n\\t)
}


SOURCES += \
        main.cpp \
        shadexicon.cpp \
        shadex.cpp \
        debugwidget.cpp \
        preferencesdialog.cpp \
        preferences.cpp \
        titlebar.cpp \
        titlebarwidget.cpp \
        windowctrl.cpp \
        windowctrl-win.cpp

HEADERS += \
        debug.h \
        shadexicon.h \
        shadex.h \
        debugwidget.h \
        preferencesdialog.h \
        preferences.h \
        titlebar.h \
        titlebarwidget.h \
        windowctrl.h \
        windowctrl-win.h

FORMS += \
        debugwidget.ui \
        preferences.ui \
        aboutdialog.ui

# Default rules for deployment.
!isEmpty(target.path): INSTALLS += target


RESOURCES += \
        Shade-X.qrc

win32:RC_FILE += \
        Shade-X.rc
