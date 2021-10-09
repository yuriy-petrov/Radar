QT -= gui

CONFIG += console
CONFIG -= app_bundle

include (../Radar.pri)
include(../RadarCore.pri)

SOURCES += \
        RadarAirObject.cpp \
        RadarApplication.cpp \
        main.cpp

HEADERS += \
    RadarAirObject.h \
    RadarApplication.h
