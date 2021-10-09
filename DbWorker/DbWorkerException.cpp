#include "DbWorkerException.h"

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

DbWorkerException::DbWorkerException( const QSqlDatabase & db )
  : std::runtime_error( db.lastError().text().toStdString() )
{}

DbWorkerException::DbWorkerException( const QSqlQuery & query )
  : std::runtime_error( QString( "%1 : %2" ).arg( query.lastError().text() ).arg( query.lastQuery() ).toStdString() )
{}
