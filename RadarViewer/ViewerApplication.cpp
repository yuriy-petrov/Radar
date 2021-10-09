#include "ViewerApplication.h"

#include <DbWorkerFactory.h>
#include <QDebug>

ViewerApplication::ViewerApplication( int & argc, char ** argv )
  : QApplication( argc, argv )
  , _dbWorker( DbWorkerFactory::createDbWorker( DbWorkerFactory::Type::SQLite, { "storage.sqlite", "", "", "", 0 } ) )
  , _trackController( _mainWindow.trackScene() )
  , _historyTrackController( _mainWindow.historyTrackScene() )
{
    _historyTrackController.setTrackFade( false );

    connect( &_mainWindow, &MainWindow::trackSelected, this, [this]( const QString & id ) {
        _trackController.setSelected( id );
    } );

    connect( &_mainWindow, &MainWindow::loadHistoryClicked, this, &ViewerApplication::loadHistory );

    connect( &_mainWindow, &MainWindow::historyTrackSelected, this, [this]( const QString & id ) {
        _historyTrackController.setActiveTracks( { id } );
        AirObjectHistory trackHistory;
        auto track = _mainWindow.historyModel().track( id );
        if ( !track.isEmpty() ) {
            trackHistory.insert( track.first().id(), track );
        }
        _historyTrackController.setAirObjects( trackHistory );
    } );

    _mainWindow.show();
    update();

    connect( &_mainWindow, &MainWindow::updateIntervalChanged, this, &ViewerApplication::setUpdateInterval );
    setUpdateInterval( _mainWindow.updateInterval() );
}

void ViewerApplication::timerEvent( QTimerEvent * event )
{
    Q_UNUSED( event );
    update();
}

void ViewerApplication::update()
{
    const auto trackingObjects = _dbWorker->loadTrackingAirObjects();
    QStringList ids;
    QDateTime lastTime;
    std::transform( trackingObjects.cbegin(),
                    trackingObjects.cend(),
                    std::back_inserter( ids ),
                    [&lastTime]( const AirObject & airObject ) {
                        if ( lastTime.isNull() ) {
                            lastTime = airObject.timeStamp();
                        } else if ( airObject.timeStamp() > lastTime ) {
                            lastTime = airObject.timeStamp();
                        }
                        return airObject.id();
                    } );
    _trackController.setActiveTracks( ids );
    _mainWindow.trackModel().setAirObjects( trackingObjects );
    if ( !ids.isEmpty() ) {
        auto beginTime = _lastTrackTime.isNull() ? QDateTime() : _lastTrackTime;
        auto endTime   = _lastTrackTime.isNull() ? QDateTime() : QDateTime::currentDateTime();
        auto tracks    = _dbWorker->loadAirObjectsHistory( beginTime, endTime, ids );
        if ( !tracks.isEmpty() ) {
            _lastTrackTime = lastTime.addMSecs( 1 );
            _trackController.setAirObjects( tracks );
        }
    }
}

void ViewerApplication::loadHistory( const QDateTime & beginTime, const QDateTime & endTime )
{
    auto history = _dbWorker->loadAirObjectsHistory( beginTime, endTime );
    _mainWindow.historyModel().setAirObjectHistory( history );
}

void ViewerApplication::setUpdateInterval( int value )
{
    qDebug() << "Update interval:" << value << "ms";
    if ( _updateTimerId != -1 ) {
        killTimer( _updateTimerId );
    }
    _updateTimerId = startTimer( value );
}
