#-------------------------------------------------
#
# Project created by QtCreator 2016-09-02T07:28:58
#
#-------------------------------------------------

QT       += core gui
QT       += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = excel
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    settings.cpp \
    bestscores.cpp

HEADERS  += mainwindow.h \
    settings.h \
    bestscores.h

FORMS    += mainwindow.ui \
    settings.ui \
    bestscores.ui
