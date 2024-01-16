#include "PekaEDSMainView.h"

PekaEDSMainView::PekaEDSMainView(PekaEDSWindow* mainWindow) : QWidget(mainWindow), mainWindow(mainWindow) {
	setupUI();
}

PekaEDSMainView& PekaEDSMainView::setTilesetView(TilesetView* newTilesetView) {
	tilesetView = newTilesetView;

	dockTilesetView->setWidget(tilesetView);

	return *this;
}

PekaEDSMainView& PekaEDSMainView::setMapView(MapView* newMapView) {
	mapView = newMapView;

	mapViewScrollArea->setMapView(mapView);

	return *this;
}

PekaEDSMainView& PekaEDSMainView::setMapMetadataView(MapMetadataView* newMapMetadataView) {
	mapMetadataView = newMapMetadataView;

	return *this;
}

PekaEDSMainView& PekaEDSMainView::setSpriteListView(SpriteListView* newSpriteListView) {
	spriteListView = newSpriteListView;

	return *this;
}

PekaEDSMainView& PekaEDSMainView::setMiniMapView(MiniMapView* newMiniMapView) {
	miniMapView = newMiniMapView;

	dockMiniMapView->setWidget(miniMapView);

	return *this;
}

void PekaEDSMainView::setMapMetadataComponent(MapMetadataComponent* component) {
	mapMetadataComponent = component;

	// TODO This is nonsense
	dockMetadata->setWidget(static_cast<MapMetadataView*>(mapMetadataComponent));
}

void PekaEDSMainView::setupUI() {
	mapViewScrollArea = new MapViewScrollArea(mainWindow);

	mainLayout = new QHBoxLayout;
	mainLayout->setContentsMargins(0, 0, 0, 0);
	mainLayout->addWidget(mapViewScrollArea);

	setLayout(mainLayout);

	dockTilesetView = new QDockWidget("Tileset", mainWindow);
	dockTilesetView->setAllowedAreas(Qt::AllDockWidgetAreas);

	dockMiniMapView = new QDockWidget("Minimap", mainWindow);
	dockMiniMapView->setAllowedAreas(Qt::AllDockWidgetAreas);

	dockMetadata = new QDockWidget("Map metadata", mainWindow);
	dockMetadata->setAllowedAreas(Qt::AllDockWidgetAreas);

	mainWindow->addDockWidget(Qt::LeftDockWidgetArea, dockTilesetView);
	mainWindow->addDockWidget(Qt::RightDockWidgetArea, dockMiniMapView);
	mainWindow->addDockWidget(Qt::RightDockWidgetArea, dockMetadata);
	mainWindow->tabifyDockWidget(dockMiniMapView, dockMetadata);
}

void PekaEDSMainView::create() {
}
