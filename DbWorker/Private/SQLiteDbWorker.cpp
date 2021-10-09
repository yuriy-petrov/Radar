#include "SQLiteDbWorker.h"
#include "DbWorkerException.h"

#include <QElapsedTimer>
#include <QLoggingCategory>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>

namespace {

Q_LOGGING_CATEGORY( LOG, "SQLiteDbWorker" )

QDebug operator<<( QDebug debug, const QElapsedTimer & timer )
{
    QDebugStateSaver dss( debug );
    return debug.nospace() << "[" << qSetRealNumberPrecision( 3 ) << timer.elapsed() / 1000.0 << "s]";
}

} // namespace

SQLiteDbWorker::SQLiteDbWorker( const ConnectionSettings & connection )
  : _db( QSqlDatabase::addDatabase( "QSQLITE" ) )
{
    _db.setDatabaseName( connection.database );
    if ( _db.open() ) {
        QSqlQuery query( _db );
        if ( !query.exec( QStringLiteral( "CREATE TABLE IF NOT EXISTS airobjects ( "
                                          "id TEXT NOT NULL PRIMARY KEY, "
                                          "state TEXT NOT NULL, "
                                          "ts TIMESTAMP NOT NULL, "
                                          "x REAL, "
                                          "y REAL "
                                          ")" ) ) ) {
            throw DbWorkerException( query );
        }
        if ( !query.exec( QStringLiteral( "CREATE TABLE IF NOT EXISTS airobjects_history ( "
                                          "id TEXT NOT NULL, "
                                          "state TEXT NOT NULL, "
                                          "ts TIMESTAMP NOT NULL, "
                                          "x REAL, "
                                          "y REAL, "
                                          "FOREIGN KEY (id) REFERENCES airobjects (id) "
                                          ")" ) ) ) {
            throw DbWorkerException( query );
        }
        if ( !query.exec( QStringLiteral( "CREATE TRIGGER IF NOT EXISTS tr_ai_airobjects "
                                          "AFTER INSERT "
                                          "ON airobjects "
                                          "BEGIN "
                                          "    INSERT INTO airobjects_history (id, state, ts, x, y) "
                                          "    VALUES (NEW.id, NEW.state, NEW.ts, NEW.x, NEW.y); "
                                          "END" ) ) ) {
            throw DbWorkerException( query );
        }
        if ( !query.exec( QStringLiteral( "CREATE TRIGGER IF NOT EXISTS tr_au_airobjects "
                                          "AFTER UPDATE "
                                          "ON airobjects "
                                          "BEGIN "
                                          "    INSERT INTO airobjects_history (id, state, ts, x, y) "
                                          "    VALUES (NEW.id, NEW.state, NEW.ts, NEW.x, NEW.y); "
                                          "END" ) ) ) {
            throw DbWorkerException( query );
        }
    } else {
        throw DbWorkerException( _db );
    }
}

SQLiteDbWorker::~SQLiteDbWorker()
{}

void SQLiteDbWorker::saveAirObjects( const QList<AirObject> & airObjects )
{
    QElapsedTimer timer;
    timer.start();
    if ( !_db.transaction() ) {
        throw DbWorkerException( _db );
    }
    for ( const auto & airObject : airObjects ) {
        QSqlQuery query( _db );
        if ( !query.prepare( QStringLiteral( "INSERT INTO airobjects (id, state, ts, x, y) "
                                             "VALUES (:id, :state, :ts, :x, :y) "
                                             "ON CONFLICT (id) DO UPDATE "
                                             "SET "
                                             "state = :state, "
                                             "ts = :ts, "
                                             "x = :x, "
                                             "y = :y" ) ) ) {
            if ( !_db.rollback() ) {
                throw DbWorkerException( _db );
            }
            throw DbWorkerException( query );
        }
        query.bindValue( ":id", airObject.id() );
        query.bindValue( ":state", AirObject::stateToString( airObject.state() ) );
        query.bindValue( ":ts", airObject.timeStamp() );
        query.bindValue( ":x", airObject.pos().x() );
        query.bindValue( ":y", airObject.pos().y() );
        if ( !query.exec() ) {
            if ( !_db.rollback() ) {
                throw DbWorkerException( _db );
            }
            throw DbWorkerException( query );
        }
    }
    if ( !_db.commit() ) {
        throw DbWorkerException( _db );
    }
    qCDebug( LOG ).nospace() << "Saved " << airObjects.size() << " rows " << timer;
}

QList<AirObject> SQLiteDbWorker::loadTrackingAirObjects()
{
    QElapsedTimer timer;
    timer.start();

    QList<AirObject> airObjects;

    QSqlQuery query( _db );
    if ( !query.exec( QStringLiteral( "SELECT * FROM airobjects "
                                      "WHERE state != '%2'" )
                        .arg( AirObject::stateToString( AirObject::State::Lost ) ) ) ) {
        throw DbWorkerException( query );
    }

    while ( query.next() ) {
        auto record = query.record();
        airObjects << AirObject( record.value( "id" ).toString(),
                                 AirObject::stateFromString( record.value( "state" ).toString() ),
                                 record.value( "ts" ).toDateTime(),
                                 QPointF( record.value( "x" ).toDouble(), record.value( "y" ).toDouble() ) );
    }

    qCDebug( LOG ).nospace() << "Loaded " << airObjects.size() << " rows " << timer;

    return airObjects;
}

AirObjectHistory SQLiteDbWorker::loadAirObjectsHistory( const QDateTime & beginTime,
                                                        const QDateTime & endTime,
                                                        const QStringList & ids )

{
    QElapsedTimer timer;
    timer.start();

    int rows = 0;
    AirObjectHistory history;

    QSqlQuery query( _db );

    auto queryText = QStringLiteral( "SELECT * FROM airobjects_history" );
    QString whereStatement;
    QVariantMap bindValues;
    if ( !beginTime.isNull() && !endTime.isNull() ) {
        whereStatement = QStringLiteral( "ts >= :bt AND ts <= :et" );
        bindValues.insert( ":bt", beginTime );
        bindValues.insert( ":et", endTime );
    }
    if ( !ids.isEmpty() ) {
        if ( !whereStatement.isEmpty() ) {
            whereStatement += QStringLiteral( " AND " );
        }
        QStringList idPlaceholders;
        for ( auto it = ids.constBegin(); it != ids.constEnd(); ++it ) {
            idPlaceholders += ":id" + QString::number( std::distance( ids.constBegin(), it ) );
            bindValues.insert( idPlaceholders.last(), *it );
        }
        whereStatement += " id IN (" + idPlaceholders.join( ", " ) + ")";
    }
    if ( !whereStatement.isEmpty() ) {
        queryText += " WHERE " + whereStatement;
    }
    queryText += " ORDER BY ts";
    if ( !query.prepare( queryText ) ) {
        throw DbWorkerException( query );
    }
    for ( auto it = bindValues.cbegin(); it != bindValues.cend(); ++it ) {
        query.bindValue( it.key(), it.value() );
    }
    if ( !query.exec() ) {
        throw DbWorkerException( query );
    }

    while ( query.next() ) {
        rows++;
        auto record = query.record();
        AirObject airObject( record.value( "id" ).toString(),
                             AirObject::stateFromString( record.value( "state" ).toString() ),
                             record.value( "ts" ).toDateTime(),
                             QPointF( record.value( "x" ).toDouble(), record.value( "y" ).toDouble() ) );
        auto it = history.find( airObject.id() );
        if ( it == history.end() ) {
            it = history.insert( airObject.id(), {} );
        }
        it.value() << airObject;
    }

    qCDebug( LOG ).nospace() << "Loaded " << rows << " rows " << timer;

    return history;
}
