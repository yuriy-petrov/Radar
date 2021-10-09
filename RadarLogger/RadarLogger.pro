QT -= gui

CONFIG += console
CONFIG -= app_bundle

include(../Radar.pri)
include(../DbWorker.pri)

SOURCES += \
    RadarLoggerApplication.cpp \
    main.cpp

HEADERS += \
    RadarLoggerApplication.h
