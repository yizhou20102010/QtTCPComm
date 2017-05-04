#-------------------------------------------------
#
# Project created by QtCreator 2017-05-03T11:57:01
#
#-------------------------------------------------

QT       += core gui
QT += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QtPLCCommTest
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    plccomm.cpp

HEADERS  += mainwindow.h \
    plccomm.h

FORMS    += mainwindow.ui

CONFIG +=console
