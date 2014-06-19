QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += static

TARGET = micropaint

TEMPLATE = app

RESOURCES += \
    micropaint_icones.qrc

FORMS += \
    mainwindow.ui

SOURCES += \
    mainwindow.cpp \
    main.cpp \
    graphicsscene.cpp

HEADERS += \
    mainwindow.h \
    graphicsscene.h
