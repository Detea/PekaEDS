#pragma once
#include <QWidget>
#include <QDockWidget>
#include <QBoxLayout>
#include <QScrollArea>

#include "PekaEDSWindow.h"
#include "TilesetView.h"
#include "RegionView.h"
#include "MapMetadataView.h"
#include "MiniMapView.h"
#include "SpriteView.h"
#include "RegionsListView.h"
#include "RegionMetadataView.h"

#include "MapMetadataComponent.h"

// TODO [CLEANUP] Remove these (all?) Check which ones are necessary
class PekaEDSWindow;
class TilesetView;
class RegionView;
class MapMetadataView;
class SpriteListView;
class MiniMapView;
class MapViewScrollArea;

/*
	This class holds all the widgets (or "components" as they will be referred to in this project) the user sees on the screen.

	A "component", in the context of this project, is any QWidget that displays and lets the user manipulate any map data.
	Components may connect to any of the PekaEDSWindow signals to be notified of any map data changes.

	The main menu is not a component and is thus not a part of this class. A toolbar however would be.

	The idea of this class is to have different main views for different map formats.
	A new map format might display some extra data in the MapMetadataView or use a different MapView.
	It's easy to construct a new "view", via dependency injection.

	The terms "view" and "component" might be used interchangeably.
	A "component" has to consist of atleast a ComponentNameView class, which inherits from QWidgets.
	A "component" may be broken down into smaller parts to make it easier to maintain.
	The TilesetView component uses a TilesetRenderer to separate it's logic from it's drawing. This is encouraged, as long as it is not overdone.

	The "view" is the "components" main QWidget. A "component" is either just a single "view" or a collection of multiple classes and one "view".

	The layout will remain the same for all views for now, but that might change later.

	See the "Components" section in the private area of this class for examples.
*/
class PekaEDSMainView : public QWidget {
public:
	PekaEDSMainView(PekaEDSWindow* parent);

	PekaEDSMainView& setTilesetView(TilesetView* newTilesetView);
	PekaEDSMainView& setMapView(RegionView* newMapView);
	PekaEDSMainView& setMapMetadataView(MapMetadataView* newMapMetadataView);
	PekaEDSMainView& setSpriteView(SpriteView* newSpriteView);
	PekaEDSMainView& setMiniMapView(MiniMapView* newMiniMapView);
    PekaEDSMainView& setRegionView(RegionListView* regionView);
    PekaEDSMainView& setRegionMetadataView(RegionMetadataView* regionMetadataView);

	void create();

private:
	void setupUI();

private:
	PekaEDSWindow* mainWindow = nullptr;

	// TODO Find the best layout for this
	QHBoxLayout* mainLayout = nullptr;

	// Components
	TilesetView* tilesetView = nullptr;

	RegionView* mapView = nullptr;

	MapMetadataView* mapMetadataView = nullptr;
	SpriteView* spriteView = nullptr;

    QScrollArea* miniMapScrollArea = nullptr;
    MiniMapView* miniMapView = nullptr;

	MapMetadataComponent* mapMetadataComponent = nullptr;

	QDockWidget* dockTilesetView = nullptr;
	QDockWidget* dockTabWidget = nullptr;
	QDockWidget* dockMiniMapView = nullptr;
	QDockWidget* dockMetadata = nullptr;
	QDockWidget* dockSpriteView = nullptr;
    QDockWidget* dockRegionView = nullptr;
    QDockWidget* dockRegionMetadataView = nullptr;
};

