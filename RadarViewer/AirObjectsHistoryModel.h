#pragma once

#include <AirObject.h>
#include <QAbstractListModel>

class AirObjectsHistoryModel : public QAbstractListModel
{
public:
    explicit AirObjectsHistoryModel( QObject * parent = nullptr );

    void setAirObjectHistory( const AirObjectHistory & airObjectHistory );

    QList<AirObject> track( const QModelIndex & index ) const;
    QList<AirObject> track( const QString & id ) const;

    // Header:
    QVariant headerData( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const override;

    // Basic functionality:
    int rowCount( const QModelIndex & parent = QModelIndex() ) const override;

    QVariant data( const QModelIndex & index, int role = Qt::DisplayRole ) const override;

private:
    AirObjectHistory _airObjectHistory;
};
