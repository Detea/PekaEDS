#include "PekaEDSMainView.h"
#include <qdockwidget.h>

PekaEDSMainView::PekaEDSMainView(PekaEDSWindow* mainWindow) : QWidget(mainWindow), mainWindow(mainWindow) {
    setupUI();
}

// TODO Clean this class up. Or rather remove it?
PekaEDSMainView& PekaEDSMainView::setTilesetView(TilesetView* newTilesetView) {
	tilesetView = newTilesetView;

	dockTilesetView->setWidget(tilesetView);

	return *this;
}

PekaEDSMainView& PekaEDSMainView::setMapView(RegionView* newMapView) {
	mapView = newMapView;

    //mapViewScrollArea->setMapView(mapView);

	return *this;
}

PekaEDSMainView& PekaEDSMainView::setSpriteView(SpriteView* newSpriteView) {
	spriteView = newSpriteView;

	dockSpriteView->setWidget(spriteView);

	return *this;
}

PekaEDSMainView& PekaEDSMainView::setMiniMapView(MiniMapView* newMiniMapView) {
	miniMapView = newMiniMapView;

    miniMapScrollArea->setAlignment(Qt::AlignCenter);
    miniMapScrollArea->setWidget(miniMapView);
    dockMiniMapView->setWidget(miniMapScrollArea);

	return *this;
}

PekaEDSMainView& PekaEDSMainView::setMapMetadataView(MapMetadataView* view) {
    mapMetadataView = view;

    dockMetadata->setWidget(mapMetadataView);

    return *this;
}

PekaEDSMainView& PekaEDSMainView::setRegionView(RegionListView* regionView) {
    dockRegionView->setWidget(regionView);

    return *this;
}

PekaEDSMainView& PekaEDSMainView::setRegionMetadataView(RegionMetadataView* regionMetadataView) {
    dockRegionMetadataView->setWidget(regionMetadataView);

    return *this;
}

void PekaEDSMainView::setupUI() {
    dockTilesetView = new QDockWidget("Tileset", mainWindow);
    dockTilesetView->setAllowedAreas(Qt::AllDockWidgetAreas);

    dockMiniMapView = new QDockWidget("Minimap", mainWindow);
    dockMiniMapView->setAllowedAreas(Qt::AllDockWidgetAreas);

    dockMetadata = new QDockWidget("Map metadata", mainWindow);
    dockMetadata->setAllowedAreas(Qt::AllDockWidgetAreas);

    dockSpriteView = new QDockWidget("Sprites", mainWindow);
    dockSpriteView->setAllowedAreas(Qt::AllDockWidgetAreas);

    dockRegionView = new QDockWidget("Regions", mainWindow);
    dockRegionView->setAllowedAreas(Qt::AllDockWidgetAreas);

    dockRegionMetadataView = new QDockWidget("Region Metadata", mainWindow);
    dockRegionMetadataView->setAllowedAreas(Qt::AllDockWidgetAreas);

    miniMapScrollArea = new QScrollArea(mainWindow);
    miniMapScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    miniMapScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
}

void PekaEDSMainView::create() {
    mainLayout = new QHBoxLayout;
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addWidget(mapView);

    setLayout(mainLayout);

    mainWindow->addDockWidget(Qt::LeftDockWidgetArea, dockTilesetView);
    mainWindow->addDockWidget(Qt::RightDockWidgetArea, dockMetadata);
    mainWindow->addDockWidget(Qt::RightDockWidgetArea, dockSpriteView);
    mainWindow->addDockWidget(Qt::RightDockWidgetArea, dockMiniMapView);
    mainWindow->addDockWidget(Qt::RightDockWidgetArea, dockRegionView);
    mainWindow->addDockWidget(Qt::RightDockWidgetArea, dockRegionMetadataView);

    mainWindow->tabifyDockWidget(dockMetadata, dockSpriteView);
    mainWindow->tabifyDockWidget(dockMetadata, dockRegionMetadataView);
    mainWindow->tabifyDockWidget(dockMetadata, dockRegionView);
    mainWindow->setTabPosition(Qt::AllDockWidgetAreas, QTabWidget::TabPosition::North);
}
