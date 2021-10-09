#pragma once

#include "RadarAirObject.h"

#include <QCoreApplication>

class RadarApplication : public QCoreApplication
{
public:
    RadarApplication( int & argc, char ** argv );

protected:
    // QObject interface
    void timerEvent( QTimerEvent * event );

private:
    void generateTargets();
    RadarAirObject generateTarget();

    const int MaxTargets       = 10;
    const int MaxTargetUpdates = 5;

    QList<RadarAirObject> _targets;
};
