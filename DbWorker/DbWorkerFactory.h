#pragma once

#include "DbWorkerInterface.h"

#include <memory>

struct DbWorkerFactory
{
    using ConnectionSettings = DbWorkerInterface::ConnectionSettings;

    enum class Type
    {
        SQLite
    };

    static std::unique_ptr<DbWorkerInterface> createDbWorker( Type type, const ConnectionSettings & connection );
};
