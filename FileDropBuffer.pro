#-------------------------------------------------
#
# Project created by QtCreator 2016-06-18T23:36:40
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = "File drop buffer"
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    dropzonewidget.cpp

HEADERS  += mainwindow.h \
    dropzonewidget.h

FORMS    += mainwindow.ui

RESOURCES += \
    mainwindow.qrc

macx:ICON = mac/icon.icns
win32:RC_ICONS += win/icon.ico
