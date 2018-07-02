#-------------------------------------------------
#
# Project created by QtCreator 2018-06-26T10:10:33
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ManifestAssistant
TEMPLATE = app


SOURCES += main.cpp\
        dialog.cpp \
    backendthread.cpp \
    dot.cpp \
    component.cpp \
    cmdexecutor.cpp

HEADERS  += dialog.h \
    backendthread.h \
    dot.h \
    component.h \
    cmdexecutor.h

FORMS    += dialog.ui
