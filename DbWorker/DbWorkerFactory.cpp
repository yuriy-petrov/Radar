#include "DbWorkerFactory.h"
#include "Private/SQLiteDbWorker.h"

std::unique_ptr<DbWorkerInterface> DbWorkerFactory::createDbWorker( Type type, const ConnectionSettings & connection )
{
    switch ( type ) {
        case Type::SQLite:
            return std::make_unique<SQLiteDbWorker>( connection );
        default:
            return {};
    }
}
