#-------------------------------------------------
#
# Project created by QtCreator 2018-04-09T18:55:32
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Imaginote
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
    analysis.cpp \
    coordinates.cpp \
        main.cpp \
        mainwindow.cpp \
    simplespaceobject.cpp \
    space.cpp \
    track.cpp \
    viewingcone.cpp \
    mapwidget.cpp \
    segment.cpp \
    line.cpp \
    coordinates2d.cpp \
    angle.cpp \
    genericspaceplayer.cpp \
    sonarspaceplayer.cpp

HEADERS += \
    analysis.h \
        mainwindow.h \
    coordinates.h \
    space.h \
    angle.h \
    newspaceplayer.h \
    simplespaceobject.h \
    track.h \
    viewingcone.h \
    mapwidget.h \
    segment.h \
    line.h \
    coordinates2d.h \
    nvgmath.h \
    genericspaceplayer.h \
    sonarspaceplayer.h

FORMS += \
        mainwindow.ui \
    mapwidget.ui

CONFIG += \
    c++17

win32: INCLUDEPATH += 'C:\Program Files (x86)\OpenAL 1.1 SDK\include'

unix: LIBS += -lopenal
win32: LIBS += 'C:\Program Files (x86)\OpenAL 1.1 SDK\libs\Win64\OpenAL32.lib'
