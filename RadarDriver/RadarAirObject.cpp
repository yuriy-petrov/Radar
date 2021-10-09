#include "RadarAirObject.h"

#include <QDebug>
#include <QJsonDocument>
#include <QtMath>
#include <iostream>

namespace {

template<typename T>
int signOf( T val )
{
    return ( T( 0 ) < val ) - ( val < T( 0 ) );
}

} // namespace

RadarAirObject::RadarAirObject( const QString & id, const QPointF & startPos, const QPointF & endPos, double speed )
  : AirObject( id, AirObject::State::Found, QDateTime::currentDateTime(), startPos )
  , _endPos( endPos )
{
    // вектор скорости рассчитывается как нормализованный вектор направления движения умноженный на скорость
    _speedVector = _endPos - pos();
    auto length  = qSqrt( _speedVector.x() * _speedVector.x() + _speedVector.y() * _speedVector.y() );
    _speedVector /= length;
    _speedVector *= speed;
}

void RadarAirObject::update( const QDateTime & timeStamp )
{
    switch ( state() ) {
        case AirObject::State::Found: {
            print();
            setState( AirObject::State::Leading );
            break;
        }
        case AirObject::State::Leading: {
            double deltaTime = this->timeStamp().msecsTo( timeStamp ) / 1000.0;
            setPos( pos() + _speedVector * deltaTime );
            setTimestamp( timeStamp );

            // для определения окончания движения используются знаки компонентов вектора скорости и вектора расстояния
            // от конечной точки до текущего положения цели
            auto distance = _endPos - pos();
            if ( signOf( distance.x() ) != signOf( _speedVector.x() )
                 || signOf( distance.y() ) != signOf( _speedVector.y() ) ) {
                setState( AirObject::State::Lost );
            }

            print();
            break;
        }
        case AirObject::State::Lost:;
    }
}

void RadarAirObject::print()
{
    std::cout << QJsonDocument( toJSon() ).toJson( QJsonDocument::Compact ).constData() << std::endl;
}
