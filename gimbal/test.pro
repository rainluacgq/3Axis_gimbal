#-------------------------------------------------
#
# Project created by QtCreator 2018-03-31T11:23:10
#
#-------------------------------------------------

QT       += core gui
QT       +=serialport
CONFIG += qwt
DEFINES += QT_DLL QWT_DLL
LIBS += -L"D:\qtfile\5.10.1\mingw53_32\lib" -lqwtd
LIBS += -L"D:\qtfile\5.10.1\mingw53_32\lib" -lqwt
INCLUDEPATH += D:\qtfile\5.10.1\mingw53_32\include\qwt
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = test
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
        main.cpp \
        mainwindow.cpp

HEADERS += \
        mainwindow.h \
    timescaledraw.h

FORMS += \
        mainwindow.ui

RESOURCES += \
    icon.qrc
