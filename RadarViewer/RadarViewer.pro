QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = app

include(../Radar.pri)
include(../DbWorker.pri)

SOURCES += \
    AirObjectModel.cpp \
    AirObjectsHistoryModel.cpp \
    MainWindow.cpp \
    Track.cpp \
    TrackController.cpp \
    TrackHistoryModel.cpp \
    ViewerApplication.cpp \
    main.cpp

HEADERS += \
    AirObjectModel.h \
    AirObjectsHistoryModel.h \
    MainWindow.h \
    Track.h \
    TrackController.h \
    TrackHistoryModel.h \
    ViewerApplication.h

FORMS += \
    MainWindow.ui

RESOURCES += \
