#include "Track.h"

#include <QGraphicsScene>

namespace {

const double PlotSize          = 10;
const double SelectedPenWidth  = 0.0;
const int UnselectedPenWidth   = 0.0;
const auto SelectedEdgeColor   = Qt::red;
const auto UnselectedEdgeColor = Qt::lightGray;
const double MaxVisiblePlots   = 30.0;

template<class T, template<class> class C>
double getOpacity( typename C<T>::const_iterator it, const C<T> & container, double maxVisibleElements, bool isFade )
{
    return isFade
             ? qBound( 0.0, maxVisibleElements - (double)std::distance( it, container.cend() ), maxVisibleElements )
                 / maxVisibleElements
             : 1.0;
}

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

        if ( _plots.size() > 1 ) {
            _edges << _scene->addLine(
              QLineF( _plots.last()->rect().center(), ( *( _plots.end() - 2 ) )->rect().center() ),
              QPen( _isSelected ? SelectedEdgeColor : UnselectedEdgeColor,
                    _isSelected ? SelectedPenWidth : UnselectedPenWidth ) );
        }
        _text->setPos( airObject.pos() * 1000 );
    }

    updatePlots();
}

void Track::setSelected( bool value )
{
    _isSelected = value;

    for ( auto plot : _plots ) {
        auto pen = plot->pen();
        pen.setWidthF( _isSelected ? SelectedPenWidth : UnselectedPenWidth );
        plot->setPen( pen );
    }

    for ( auto edge : _edges ) {
        auto pen = edge->pen();
        pen.setWidthF( _isSelected ? SelectedPenWidth : UnselectedPenWidth );
        pen.setColor( _isSelected ? SelectedEdgeColor : UnselectedEdgeColor );
        edge->setPen( pen );
    }
}

void Track::setFade( bool value )
{
    _isFade = value;
    updatePlots();
}

bool Track::fadePlotsAndHasVisible()
{
    bool hasVisible = false;
    for ( auto it = _plots.cbegin(); it != _plots.cend(); ++it ) {
        ( *it )->setPen( QPen( Qt::lightGray, 0.0 ) );
        auto opacity = qBound( 0.0, ( *it )->opacity() - 0.3, 1.0 );
        ( *it )->setOpacity( opacity );
        if ( opacity > 0.0 ) {
            hasVisible = true;
        }
    }
    for ( auto it = _edges.cbegin(); it != _edges.cend(); ++it ) {
        auto opacity = qBound( 0.0, ( *it )->opacity() - 0.3, 1.0 );
        ( *it )->setOpacity( opacity );
    }
    return hasVisible;
}

void Track::updatePlots()
{
    for ( auto it = _plots.cbegin(); it != _plots.cend(); ++it ) {
        if ( it != _plots.cend() - 1 ) {
            ( *it )->setPen( QPen( Qt::lightGray, 0.0 ) );
            ( *it )->setOpacity( getOpacity( it, _plots, MaxVisiblePlots, _isFade ) );
        } else {
            ( *it )->setPen( QPen( Qt::blue, 0.0 ) );
            ( *it )->setOpacity( 1.0 );
        }
    }

    for ( auto it = _edges.cbegin(); it != _edges.cend(); ++it ) {
        if ( it != _edges.cend() - 1 ) {
            ( *it )->setOpacity( getOpacity( it, _edges, MaxVisiblePlots, _isFade ) );
        }
    }
}
