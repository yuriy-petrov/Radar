#pragma once

#include "DbWorkerInterface.h"

#include <QSqlDatabase>
#include <memory>

class SQLiteDbWorker : public DbWorkerInterface
{
public:
    SQLiteDbWorker( const ConnectionSettings & connection );
    ~SQLiteDbWorker();

    void saveAirObjects( const QList<AirObject> & airObjects ) override;
    QList<AirObject> loadTrackingAirObjects() override;
    AirObjectHistory loadAirObjectsHistory( const QDateTime & beginTime,
                                            const QDateTime & endTime,
                                            const QStringList & ids ) override;

private:
    QSqlDatabase _db;
};
