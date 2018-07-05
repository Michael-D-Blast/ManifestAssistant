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
    cmdexecutor.cpp \
    gitexecutor.cpp \
    fileeditor.cpp \
    manifesteditor.cpp \
    myerror.cpp \
    taglineedit.cpp \
    branchdialog.cpp \
    localcommitscheckdialog.cpp \
    pushthread.cpp

HEADERS  += dialog.h \
    backendthread.h \
    dot.h \
    component.h \
    cmdexecutor.h \
    gitexecutor.h \
    fileeditor.h \
    manifesteditor.h \
    myerror.h \
    taglineedit.h \
    branchdialog.h \
    localcommitscheckdialog.h \
    pushthread.h

FORMS    +=
