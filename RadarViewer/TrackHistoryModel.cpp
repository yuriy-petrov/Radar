#include "TrackHistoryModel.h"

TrackHistoryModel::TrackHistoryModel( QObject * parent )
  : AirObjectModel( { AirObjectModel::Timestamp, AirObjectModel::X, AirObjectModel::Y }, parent )
{}

void TrackHistoryModel::setAirObjects( const QList<AirObject> & airObjects )
{
    beginResetModel();
    _airObjects = airObjects;
    endResetModel();
}
