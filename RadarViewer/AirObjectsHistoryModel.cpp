#include "AirObjectsHistoryModel.h"

AirObjectsHistoryModel::AirObjectsHistoryModel( QObject * parent )
  : QAbstractListModel( parent )
{}

void AirObjectsHistoryModel::setAirObjectHistory( const AirObjectHistory & airObjectHistory )
{
    beginResetModel();
    _airObjectHistory = airObjectHistory;
    endResetModel();
}

QList<AirObject> AirObjectsHistoryModel::track( const QModelIndex & index ) const
{
    return index.isValid() ? ( _airObjectHistory.cbegin() + index.row() ).value() : QList<AirObject>{};
}

QList<AirObject> AirObjectsHistoryModel::track( const QString & id ) const
{
    auto it = _airObjectHistory.find( id );
    return it == _airObjectHistory.cend() ? QList<AirObject>() : it.value();
}

QVariant AirObjectsHistoryModel::headerData( int section, Qt::Orientation orientation, int role ) const
{
    return QAbstractListModel::headerData( section, orientation, role );
}

int AirObjectsHistoryModel::rowCount( const QModelIndex & parent ) const
{
    Q_UNUSED( parent );
    return _airObjectHistory.size();
}

QVariant AirObjectsHistoryModel::data( const QModelIndex & index, int role ) const
{
    if ( !index.isValid() ) {
        return QVariant();
    }

    if ( role == Qt::DisplayRole ) {
        auto it = _airObjectHistory.cbegin() + index.row();
        return it.key();
    }

    return QVariant();
}
