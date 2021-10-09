QT -= gui
QT += sql

TEMPLATE = lib

CONFIG += console
CONFIG -= app_bundle

include (../Radar.pri)
include (../RadarCore.pri)

SOURCES += \
    DbWorkerException.cpp \
    DbWorkerFactory.cpp \
    Private/SQLiteDbWorker.cpp

HEADERS += \
    DbWorkerException.h \
    DbWorkerFactory.h \
    DbWorkerInterface.h \
    Private/SQLiteDbWorker.h
