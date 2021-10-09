QT -= gui

TEMPLATE = lib

CONFIG += console
CONFIG -= app_bundle

include (../Radar.pri)

SOURCES += \
    AirObject.cpp

HEADERS += \
    AirObject.h
