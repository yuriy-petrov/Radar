#pragma once

#include "MainWindow.h"
#include "TrackController.h"

#include <DbWorkerInterface.h>
#include <QApplication>

class ViewerApplication : public QApplication
{
public:
    ViewerApplication( int & argc, char ** argv );

protected:
    // QObject interface
    void timerEvent( QTimerEvent * event );

private:
    void update();
    void loadHistory( const QDateTime & beginTime, const QDateTime & endTime );

    MainWindow _mainWindow;
    QDateTime _lastTrackTime;
    std::unique_ptr<DbWorkerInterface> _dbWorker;
    TrackController _trackController;
    TrackController _historyTrackController;
};
