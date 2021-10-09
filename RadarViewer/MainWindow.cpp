#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QGraphicsLineItem>
#include <QSettings>

MainWindow::MainWindow( QWidget * parent )
  : QMainWindow( parent )
  , ui( new Ui::MainWindow )
{
    ui->setupUi( this );

    _trackScene.setSceneRect( QRectF( 0.0, 0.0, 1000.0, 1000.0 ) );
    ui->graphicsViewTracks->setScene( &_trackScene );

    _historyScene.setSceneRect( QRectF( 0.0, 0.0, 1000.0, 1000.0 ) );
    ui->graphicsViewHistory->setScene( &_historyScene );

    ui->tableViewTracks->setModel( &_trackModel );

    connect( ui->tableViewTracks->selectionModel(), &QItemSelectionModel::selectionChanged, this, [this]() {
        auto selection = ui->tableViewTracks->selectionModel()->selectedRows();
        emit trackSelected( _trackModel.id( selection.isEmpty() ? QModelIndex() : selection.first() ) );
    } );

    ui->listViewHistory->setModel( &_historyModel );
    connect( ui->listViewHistory->selectionModel(), &QItemSelectionModel::selectionChanged, this, [this]() {
        auto selection = ui->listViewHistory->selectionModel()->selectedRows();
        auto track     = _historyModel.track( selection.isEmpty() ? QModelIndex() : selection.first() );
        _historyTrackModel.setAirObjects( track );
        emit historyTrackSelected( track.isEmpty() ? QString() : track.first().id() );
    } );

    ui->tableViewHistory->setModel( &_historyTrackModel );

    ui->dateTimeEditFrom->setDateTime( QDateTime::currentDateTime().addSecs( -60 ) );
    ui->dateTimeEditTo->setDateTime( QDateTime::currentDateTime() );

    connect( ui->pushButtonLoad, &QPushButton::clicked, this, [this]() {
        emit loadHistoryClicked( ui->dateTimeEditFrom->dateTime(), ui->dateTimeEditTo->dateTime() );
    } );

    connect( ui->tabWidget, &QTabWidget::currentChanged, this, [this]() { fitScenesToView(); } );

    createRadarLines( _trackScene );
    createRadarLines( _historyScene );

    restoreUiState();
}

MainWindow::~MainWindow()
{
    saveUiState();
    delete ui;
}

void MainWindow::restoreUiState()
{
#ifdef Q_OS_WIN
    QSettings settings( qApp->applicationDirPath() + qAppName() + ".ini", QSettings::IniFormat );
#else
    QSettings settings;
#endif
    restoreGeometry( settings.value( "geometry", saveGeometry() ).toByteArray() );
    ui->splitter->restoreState( settings.value( ui->splitter->objectName(), ui->splitter->saveState() ).toByteArray() );
    ui->splitter_2->restoreState(
      settings.value( ui->splitter_2->objectName(), ui->splitter_2->saveState() ).toByteArray() );
    ui->tableViewTracks->horizontalHeader()->restoreState(
      settings.value( ui->tableViewTracks->objectName(), ui->tableViewTracks->horizontalHeader()->saveState() )
        .toByteArray() );
    ui->tableViewHistory->horizontalHeader()->restoreState(
      settings.value( ui->tableViewHistory->objectName(), ui->tableViewHistory->horizontalHeader()->saveState() )
        .toByteArray() );
    ui->dateTimeEditFrom->setDateTime( settings.value( "from" ).toDateTime() );
}

void MainWindow::saveUiState()
{
#ifdef Q_OS_WIN
    QSettings settings( qApp->applicationDirPath() + qAppName() + ".ini", QSettings::IniFormat );
#else
    QSettings settings;
#endif
    settings.setValue( "geometry", saveGeometry() );
    settings.setValue( ui->splitter->objectName(), ui->splitter->saveState() );
    settings.setValue( ui->splitter_2->objectName(), ui->splitter_2->saveState() );
    settings.setValue( ui->tableViewTracks->objectName(), ui->tableViewTracks->horizontalHeader()->saveState() );
    settings.setValue( ui->tableViewHistory->objectName(), ui->tableViewHistory->horizontalHeader()->saveState() );
    settings.setValue( "from", ui->dateTimeEditFrom->dateTime() );
}

void MainWindow::fitScenesToView()
{
    ui->graphicsViewTracks->fitInView( ui->graphicsViewTracks->sceneRect(), Qt::KeepAspectRatio );
    ui->graphicsViewHistory->fitInView( ui->graphicsViewHistory->sceneRect(), Qt::KeepAspectRatio );
}

void MainWindow::createRadarLines( QGraphicsScene & scene )
{
    scene.setBackgroundBrush( QColor( QStringLiteral( "#e1e1e1" ) ) );

    auto sceneRect = scene.sceneRect();
    auto center    = sceneRect.center();
    auto pen       = QPen( QColor( QStringLiteral( "#acc5a9" ) ), 0.0 );

    for ( double scale : { 0.1, 0.2, 0.5 } ) {
        auto size = sceneRect.width() * scale;
        QRectF rect( 0, 0, size, size );
        rect.moveCenter( sceneRect.center() );
        scene.addEllipse( rect, pen );
    }

    auto mainCircle = scene.addEllipse( sceneRect, pen );
    mainCircle->setFlag( QGraphicsItem::ItemClipsChildrenToShape );

    scene.addLine( center.x(), sceneRect.top(), center.x(), sceneRect.bottom(), pen );
    scene.addLine( sceneRect.left(), center.y(), sceneRect.right(), center.y(), pen );

    scene.addLine( QLineF( sceneRect.topLeft(), sceneRect.bottomRight() ), pen )->setParentItem( mainCircle );
    scene.addLine( QLineF( sceneRect.bottomLeft(), sceneRect.topRight() ), pen )->setParentItem( mainCircle );
}

QGraphicsScene * MainWindow::trackScene()
{
    return &_trackScene;
}

QGraphicsScene * MainWindow::historyTrackScene()
{
    return &_historyScene;
}

AirObjectModel & MainWindow::trackModel()
{
    return _trackModel;
}

AirObjectsHistoryModel & MainWindow::historyModel()
{
    return _historyModel;
}

void MainWindow::showEvent( QShowEvent * event )
{
    fitScenesToView();
    QMainWindow::showEvent( event );
}

void MainWindow::resizeEvent( QResizeEvent * event )
{
    fitScenesToView();
    QMainWindow::resizeEvent( event );
}
