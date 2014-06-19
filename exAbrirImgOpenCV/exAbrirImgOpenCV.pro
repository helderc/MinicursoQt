#-------------------------------------------------
#
# Project created by QtCreator 2012-05-10T13:32:43
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = exAbrirImg
TEMPLATE = app

win32 {
    INCLUDEPATH += C:/OpenCV/include/
    LIBS += -LC:/OpenCV/lib/ -llibopencv_core230 -llibopencv_highgui230 -llibopencv_imgproc230
}

unix {
    LIBS += -lopencv_core -lopencv_highgui -lopencv_imgproc
}


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

RESOURCES += \
    recursos.qrc
