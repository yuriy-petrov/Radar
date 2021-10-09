#pragma once

#include "AirObjectModel.h"

class TrackHistoryModel : public AirObjectModel
{
public:
    TrackHistoryModel( QObject * parent = nullptr );

    void setAirObjects( const QList<AirObject> & airObjects ) override;
};
