#include "RadarLoggerApplication.h"

#include <DbWorkerFactory.h>
#include <QDebug>
#include <QJsonDocument>

RadarLoggerApplication::RadarLoggerApplication( int & argc, char ** argv )
  : QCoreApplication( argc, argv )
  , _dbWorker(
      DbWorkerFactory::createDbWorker( DbWorkerFactory::Type::SQLite,
                                       DbWorkerInterface::ConnectionSettings{ "storage.sqlite", "", "", "", 0 } ) )
{
    connect( &_radarDriver, &QProcess::readyRead, this, &RadarLoggerApplication::onRadarDriverReadyRead );
    connect( &_radarDriver, &QProcess::errorOccurred, this, [this]() { qCritical() << _radarDriver.errorString(); } );

    auto trackingObjects = _dbWorker->loadTrackingAirObjects();
    for ( auto & airObject : trackingObjects ) {
        airObject.setState( AirObject::State::Lost );
    }
    _dbWorker->saveAirObjects( trackingObjects );

    _radarDriver.setProgram( "RadarDriver" );
    _radarDriver.start( QIODevice::ReadOnly );
}

void RadarLoggerApplication::onRadarDriverReadyRead()
{
    QList<AirObject> airObjects;

    _buffer.append( _radarDriver.readAllStandardOutput() );
    QTextStream stream( &_buffer, QIODevice::ReadOnly );
    QString line;
    do {
        line = stream.readLine();
        if ( !line.isEmpty() ) {
            airObjects << AirObject::fromJSon( QJsonDocument::fromJson( line.toLatin1() ).object() );
        }
    } while ( !line.isEmpty() );
    _buffer.remove( 0, stream.pos() );

    _dbWorker->saveAirObjects( airObjects );
}
