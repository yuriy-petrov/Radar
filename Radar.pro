TEMPLATE = subdirs

SUBDIRS = \
    RadarCore \
    DbWorker \
    RadarDriver \
    RadarLogger \
    RadarViewer

DbWorker.depends = RadarCore

RadarDriver.depends = RadarCore

RadarLogger.depends = DbWorker

RadarViewer.depends = DbWorker
