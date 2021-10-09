#pragma once

#include <AirObject.h>
#include <QAbstractTableModel>

class AirObjectModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    enum Column
    {
        Id,
        Timestamp,
        X,
        Y
    };

    explicit AirObjectModel( const QList<Column> & columns = { Column::Id, Column::Timestamp, Column::X, Column::Y },
                             QObject * parent              = nullptr );

    virtual void setAirObjects( const QList<AirObject> & airObjects );

    QString id( const QModelIndex & index ) const;

    // Header:
    QVariant headerData( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const override;

    // Basic functionality:
    int rowCount( const QModelIndex & parent = QModelIndex() ) const override;
    int columnCount( const QModelIndex & parent = QModelIndex() ) const override;

    QVariant data( const QModelIndex & index, int role = Qt::DisplayRole ) const override;

protected:
    QList<Column> _columns;
    QList<AirObject> _airObjects;
};
