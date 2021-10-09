#pragma once

#include "Track.h"

#include <AirObject.h>
#include <QGraphicsScene>
#include <QSet>

class TrackController
{
public:
    TrackController( QGraphicsScene * scene );

    void setActiveTracks( const QStringList & ids );
    void setAirObjects( const AirObjectHistory & airObjects );

    void setSelected( const QString & id );

    void setTrackFade( bool value );

private:
    bool _trackFade = true;
    QGraphicsScene * _scene;
    std::map<QString, std::unique_ptr<Track>> _tracks;
    QSet<QString> _lostTracks;
};
