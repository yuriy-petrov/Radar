#include "TrackController.h"

TrackController::TrackController( QGraphicsScene * scene )
  : _scene( scene )
{}

void TrackController::setActiveTracks( const QStringList & ids )
{
    for ( auto it = _tracks.begin(); it != _tracks.end(); ++it ) {
        if ( !ids.contains( it->first ) ) {
            it = _tracks.erase( it );
            if ( _tracks.empty() ) {
                break;
            }
            it--;
        }
    }
}

void TrackController::setAirObjects( const AirObjectHistory & airObjects )
{
    for ( auto it = airObjects.cbegin(); it != airObjects.cend(); ++it ) {
        auto trackIt = _tracks.find( it.key() );
        if ( trackIt == _tracks.end() ) {
            trackIt = _tracks.insert( std::make_pair( it.key(), std::make_unique<Track>( _scene ) ) ).first;
            trackIt->second->setFade( _trackFade );
        }
        trackIt->second->appendPlots( it.value() );
    }
}

void TrackController::setSelected( const QString & id )
{
    std::for_each( _tracks.cbegin(), _tracks.cend(), [&id]( const decltype( _tracks )::value_type & el ) {
        el.second->setSelected( el.first == id );
    } );
}

void TrackController::setTrackFade( bool value )
{
    _trackFade = value;
    std::for_each( _tracks.cbegin(), _tracks.cend(), [this]( const decltype( _tracks )::value_type & track ) {
        track.second->setFade( _trackFade );
    } );
}
