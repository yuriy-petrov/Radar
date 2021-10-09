#pragma once

#include "Track.h"

#include <AirObject.h>
#include <QGraphicsScene>

class TrackController
{
public:
    TrackController( QGraphicsScene * scene );

    void setActiveTracks( const QStringList & ids );
    void setAirObjects( const AirObjectHistory & airObjects );

    void setSelected( const QString & id );

private:
    QGraphicsScene * _scene;
    std::map<QString, std::unique_ptr<Track>> _tracks;
};
