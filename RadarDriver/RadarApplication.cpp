#include "RadarApplication.h"

#include <QRandomGenerator>

RadarApplication::RadarApplication( int & argc, char ** argv )
  : QCoreApplication( argc, argv )
{
    generateTargets();
    startTimer( 200 );
}

void RadarApplication::timerEvent( QTimerEvent * event )
{
    Q_UNUSED( event );
    auto timeStamp = QDateTime::currentDateTime();

    for ( auto it = _targets.begin(); it != _targets.end(); ++it ) {
        it->update( timeStamp );
        if ( it->state() == AirObject::State::Lost ) {
            it = _targets.erase( it ) - 1;
        }
    }

    generateTargets();
}

void RadarApplication::generateTargets()
{
    if ( _targets.size() < MaxTargets ) {
        _targets << generateTarget();
    }
}

RadarAirObject RadarApplication::generateTarget()
{
    auto getPoint = []() {
        return QPointF{ QRandomGenerator::global()->bounded( 0.2 ), QRandomGenerator::global()->bounded( 0.2 ) };
    };

    auto setXSide = []( QPointF & p ) {
        auto side = QRandomGenerator::global()->bounded( 0, 2 );
        if ( side == 1 ) {
            p.setX( 1.0 - p.x() );
        }
    };
    auto setYSide = []( QPointF & p ) {
        auto side = QRandomGenerator::global()->bounded( 0, 2 );
        if ( side == 1 ) {
            p.setY( 1.0 - p.y() );
        }
    };

    auto startPoint = getPoint();
    setXSide( startPoint );
    setYSide( startPoint );

    auto endPoint = getPoint();
    setXSide( endPoint );
    setYSide( endPoint );

    RadarAirObject target( QUuid::createUuid().toString( QUuid::WithoutBraces ),
                           startPoint,
                           endPoint,
                           0.03 + QRandomGenerator::global()->bounded( 0.06 ) );
    return target;
}
