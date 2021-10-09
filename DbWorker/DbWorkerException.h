#pragma once

#include <stdexcept>

class QSqlDatabase;
class QSqlQuery;

class DbWorkerException : public std::runtime_error
{
public:
    DbWorkerException( const QSqlDatabase & db );
    DbWorkerException( const QSqlQuery & query );
};
