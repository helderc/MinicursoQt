QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = micropaint_simple

TEMPLATE = app

SOURCES += \
    graphicsscene.cpp \
    mainwindow.cpp \
    main.cpp

FORMS += \
    mainwindow.ui

RESOURCES += \
    micropaint_icones.qrc

HEADERS += \
    mainwindow.h \
    graphicsscene.h
