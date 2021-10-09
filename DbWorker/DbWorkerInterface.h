#pragma once

#include <AirObject.h>

class DbWorkerInterface
{

public:
    struct ConnectionSettings
    {
        QString database;
        QString user;
        QString password;
        QString host;
        quint16 port;
    };

    virtual ~DbWorkerInterface() = default;

    virtual void saveAirObjects( const QList<AirObject> & airObjects ) = 0;

    virtual QList<AirObject> loadTrackingAirObjects() = 0;

    virtual AirObjectHistory loadAirObjectsHistory( const QDateTime & beginTime,
                                                    const QDateTime & endTime,
                                                    const QStringList & ids = {} ) = 0;
};
