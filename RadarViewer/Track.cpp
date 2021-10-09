#include "Track.h"

#include <QGraphicsScene>

namespace {

const double PlotSize          = 10;
const double SelectedPenWidth  = 0.0;
const int UnselectedPenWidth   = 0.0;
const auto SelectedEdgeColor   = Qt::red;
const auto UnselectedEdgeColor = Qt::lightGray;

} // namespace

Track::Track( QGraphicsScene * scene )
  : _scene( scene )
{}

Track::~Track()
{
    _scene->removeItem( _text );
    delete _text;

    for ( auto plot : _plots ) {
        _scene->removeItem( plot );
        delete plot;
    }

    for ( auto edge : _edges ) {
        _scene->removeItem( edge );
        delete edge;
    }
}

void Track::appendPlots( const QList<AirObject> & airObjects )
{
    for ( const auto & airObject : airObjects ) {
        if ( !_text ) {
            auto font = _scene->font();
            font.setPixelSize( 10 );
            _text = _scene->addText( airObject.id(), font );
            _text->setOpacity( 0.3 );
        }
        _plots << _scene->addRect( QRectF( airObject.pos().x() * 1000 - PlotSize / 2.0,
                                           airObject.pos().y() * 1000 - PlotSize / 2.0,
                                           PlotSize,
                                           PlotSize ) );

        setupLastPlot();
        if ( _plots.size() > 1 ) {
            ( *( _plots.end() - 2 ) )
              ->setPen( QPen( Qt::lightGray, _isSelected ? SelectedPenWidth : UnselectedPenWidth ) );
            _edges << _scene->addLine(
              QLineF( _plots.last()->rect().center(), ( *( _plots.end() - 2 ) )->rect().center() ),
              QPen( _isSelected ? SelectedEdgeColor : UnselectedEdgeColor,
                    _isSelected ? SelectedPenWidth : UnselectedPenWidth ) );
        }
        _text->setPos( airObject.pos() * 1000 );
    }
}

void Track::setSelected( bool value )
{
    _isSelected = value;

    for ( auto plot : _plots ) {
        auto pen = plot->pen();
        pen.setWidthF( _isSelected ? SelectedPenWidth : UnselectedPenWidth );
        plot->setPen( pen );
    }
    setupLastPlot();

    for ( auto edge : _edges ) {
        auto pen = edge->pen();
        pen.setWidthF( _isSelected ? SelectedPenWidth : UnselectedPenWidth );
        pen.setColor( _isSelected ? SelectedEdgeColor : UnselectedEdgeColor );
        edge->setPen( pen );
    }
}

void Track::setupLastPlot()
{
    _plots.last()->setPen( QPen( Qt::blue, _isSelected ? SelectedPenWidth : 0.0 ) );
}
