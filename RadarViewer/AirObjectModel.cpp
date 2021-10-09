#include "AirObjectModel.h"

AirObjectModel::AirObjectModel( const QList<Column> & columns, QObject * parent )
  : QAbstractTableModel( parent )
  , _columns( columns )
{}

void AirObjectModel::setAirObjects( const QList<AirObject> & airObjects )
{
    for ( auto it = _airObjects.begin(); it != _airObjects.end(); ++it ) {
        auto existingIt = std::find_if( airObjects.cbegin(), airObjects.cend(), [&it]( const AirObject & ao ) {
            return it->id() == ao.id();
        } );
        if ( existingIt == airObjects.cend() ) {
            auto row = std::distance( _airObjects.begin(), it );
            beginRemoveRows( QModelIndex(), row, row );
            it = _airObjects.erase( it ) - 1;
            endRemoveRows();
        }
    }
    for ( const auto & airObject : airObjects ) {
        auto it = std::find_if( _airObjects.begin(), _airObjects.end(), [&airObject]( const AirObject & ao ) {
            return ao.id() == airObject.id();
        } );
        if ( it != _airObjects.end() ) {
            *it      = airObject;
            auto row = std::distance( _airObjects.begin(), it );
            emit dataChanged( index( row, 0 ), index( row, columnCount() - 1 ), { Qt::DisplayRole } );
        } else {
            beginInsertRows( QModelIndex(), _airObjects.size(), _airObjects.size() );
            _airObjects << airObject;
            endInsertRows();
        }
    }
}

QString AirObjectModel::id( const QModelIndex & index ) const
{
    return index.isValid() ? _airObjects.at( index.row() ).id() : QString();
}

QVariant AirObjectModel::headerData( int section, Qt::Orientation orientation, int role ) const
{
    if ( orientation == Qt::Horizontal && role == Qt::DisplayRole ) {
        auto column = _columns.at( section );
        switch ( column ) {
            case Column::Id:
                return tr( "id" );
            case Column::Timestamp:
                return tr( "timestamp" );
            case Column::X:
                return tr( "x" );
            case Column::Y:
                return tr( "y" );
        }
    } else if ( orientation == Qt::Vertical && role == Qt::DisplayRole ) {
        return QString( "%1" ).arg( section + 1, 2, 10, QChar( '0' ) );
    }
    return QAbstractTableModel::headerData( section, orientation, role );
}

int AirObjectModel::rowCount( const QModelIndex & parent ) const
{
    Q_UNUSED( parent );
    return _airObjects.size();
}

int AirObjectModel::columnCount( const QModelIndex & parent ) const
{
    Q_UNUSED( parent );
    return _columns.size();
}

QVariant AirObjectModel::data( const QModelIndex & index, int role ) const
{
    if ( !index.isValid() ) {
        return QVariant();
    }

    const auto airObject = _airObjects.at( index.row() );
    auto column          = _columns.at( index.column() );

    if ( role == Qt::DisplayRole ) {
        switch ( column ) {
            case Column::Id:
                return airObject.id();
            case Column::Timestamp:
                return airObject.timeStamp().toString( QStringLiteral( "yyyy.dd.MM hh:mm:ss.zzz" ) );
            case Column::X:
                return airObject.pos().x();
            case Column::Y:
                return airObject.pos().y();
        }
    }

    return QVariant();
}
