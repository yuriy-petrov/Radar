#pragma once

#include "AirObjectModel.h"
#include "AirObjectsHistoryModel.h"
#include "TrackHistoryModel.h"

#include <QGraphicsScene>
#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow( QWidget * parent = nullptr );
    ~MainWindow();

    QGraphicsScene * trackScene();
    QGraphicsScene * historyTrackScene();

    AirObjectModel & trackModel();

    AirObjectsHistoryModel & historyModel();

    int updateInterval() const;

signals:
    void trackSelected( const QString & id );
    void historyTrackSelected( const QString & id );
    void loadHistoryClicked( const QDateTime & beginTime, const QDateTime & endTime );
    void updateIntervalChanged( int intervalMsec );

protected:
    // QWidget interface
    void showEvent( QShowEvent * event ) override;
    void resizeEvent( QResizeEvent * event ) override;

private:
    void restoreUiState();
    void saveUiState();
    void fitScenesToView();
    void createRadarLines( QGraphicsScene & scene );

    Ui::MainWindow * ui;
    QGraphicsScene _trackScene;
    AirObjectModel _trackModel;
    QGraphicsScene _historyScene;
    AirObjectsHistoryModel _historyModel;
    TrackHistoryModel _historyTrackModel;
};
