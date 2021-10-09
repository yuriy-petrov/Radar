#pragma once

#include <AirObject.h>
#include <QGraphicsRectItem>

class Track
{
public:
    Track( QGraphicsScene * scene );
    ~Track();

    void appendPlots( const QList<AirObject> & airObjects );

    void setSelected( bool value );

    void setFade( bool value );

private:
    void updatePlots();

    bool _isSelected = false;
    bool _isFade     = true;
    QGraphicsScene * _scene;
    QGraphicsTextItem * _text = nullptr;
    QList<QGraphicsRectItem *> _plots;
    QList<QGraphicsLineItem *> _edges;
};
