#include "AirObject.h"

#include <QMap>

namespace {
const auto KeyId    = QStringLiteral( "id" );
const auto KeyState = QStringLiteral( "state" );
const auto KeyTs    = QStringLiteral( "ts" );
const auto KeyPos   = QStringLiteral( "pos" );
const auto KeyX     = QStringLiteral( "x" );
const auto KeyY     = QStringLiteral( "y" );

const auto TimestampFormat = QStringLiteral( "dd.MM.yyyy hh:mm:ss.zzz" );

const QMap<AirObject::State, QString> StringByState{ { AirObject::State::Found, "found" },
                                                     { AirObject::State::Leading, "leading" },
                                                     { AirObject::State::Lost, "lost" } };
const QMap<QString, AirObject::State> StateByString = []() {
    QMap<QString, AirObject::State> map;
    for ( auto it = StringByState.cbegin(); it != StringByState.cend(); ++it ) {
        map.insert( it.value(), it.key() );
    }
    return map;
}();
} // namespace

AirObject::AirObject( const QString & id, State state, const QDateTime & timeStamp, const QPointF & pos )
  : _id( id )
  , _state( state )
  , _timeStamp( timeStamp )
  , _pos( pos )
{}

QJsonObject AirObject::toJSon() const
{
    QJsonObject json;

    json.insert( KeyId, _id );
    json.insert( KeyState, StringByState[_state] );
    json.insert( KeyTs, _timeStamp.toString( TimestampFormat ) );

    QJsonObject jsonPos;
    jsonPos.insert( KeyX, _pos.x() );
    jsonPos.insert( KeyY, _pos.y() );

    json.insert( KeyPos, jsonPos );

    return json;
}

AirObject AirObject::fromJSon( const QJsonObject & json )
{
    AirObject target;

    if ( json.contains( KeyId ) ) {
        target._id = json[KeyId].toString();
    }
    if ( json.contains( KeyState ) ) {
        target._state = StateByString[json[KeyState].toString()];
    }
    if ( json.contains( KeyTs ) ) {
        target._timeStamp = QDateTime::fromString( json[KeyTs].toString(), TimestampFormat );
    }
    if ( json.contains( KeyPos ) ) {
        auto jsonPos = json[KeyPos].toObject();
        if ( jsonPos.contains( KeyX ) ) {
            target._pos.setX( jsonPos[KeyX].toDouble() );
        }
        if ( jsonPos.contains( KeyY ) ) {
            target._pos.setY( jsonPos[KeyY].toDouble() );
        }
    }

    return target;
}

const QString & AirObject::id() const
{
    return _id;
}

const AirObject::State & AirObject::state() const
{
    return _state;
}

void AirObject::setState( State state )
{
    _state = state;
}

const QDateTime & AirObject::timeStamp() const
{
    return _timeStamp;
}

void AirObject::setTimestamp( const QDateTime & timeStamp )
{
    _timeStamp = timeStamp;
}

const QPointF & AirObject::pos() const
{
    return _pos;
}

void AirObject::setPos( const QPointF & pos )
{
    _pos = pos;
}

QString AirObject::stateToString( State state )
{
    return StringByState[state];
}

AirObject::State AirObject::stateFromString( const QString & string )
{
    return StateByString[string];
}
