#-------------------------------------------------
#
# Project created by QtCreator 2022-06-21T09:39:38
#
#-------------------------------------------------

QT       += core gui
QT += sql

RC_ICONS = windi.ico

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = dbtest
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    logindialog.cpp \
    modify.cpp \
    mydialog.cpp \
    register.cpp \

HEADERS  += mainwindow.h \
    logindialog.h \
    modify.h \
    mydialog.h \
    register.h

FORMS    += mainwindow.ui \
    logindialog.ui \
    modify.ui \
    register.ui \

RESOURCES += \
    recourse.qrc
