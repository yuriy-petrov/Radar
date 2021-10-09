#pragma once

#include <QDateTime>
#include <QJsonObject>
#include <QPointF>
#include <QString>

class AirObject
{
public:
    enum class State
    {
        Found,
        Leading,
        Lost
    };

    AirObject() = default;
    explicit AirObject( const QString & id, State state, const QDateTime & timeStamp, const QPointF & pos );
    virtual ~AirObject() = default;

    QJsonObject toJSon() const;
    static AirObject fromJSon( const QJsonObject & json );

    const QString & id() const;

    const State & state() const;
    void setState( State state );

    const QDateTime & timeStamp() const;
    void setTimestamp( const QDateTime & timeStamp );

    const QPointF & pos() const;
    void setPos( const QPointF & pos );

    static QString stateToString( State state );
    static State stateFromString( const QString & string );

private:
    QString _id;
    State _state;
    QDateTime _timeStamp;
    QPointF _pos;
};

using AirObjectHistory = QMap<QString, QList<AirObject>>;
