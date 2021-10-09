#pragma once

#include <DbWorkerInterface.h>
#include <QCoreApplication>
#include <QProcess>
#include <memory>

class RadarLoggerApplication : public QCoreApplication
{
public:
    RadarLoggerApplication( int & argc, char ** argv );

private:
    void onRadarDriverReadyRead();

    std::unique_ptr<DbWorkerInterface> _dbWorker;
    QProcess _radarDriver;
    QByteArray _buffer;
};
