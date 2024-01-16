#pragma once

#include <qmainwindow.h>
#include <qpointer.h>

#include <string>
#include <string_view>
#include <vector>

#include "MapView.h"
#include "MiniMapView.h"
#include "TilesetView.h"

#include "PK2MapBaseMetadata.h"
#include "PK2MapData.h"
#include "PK2SpriteBase.h"

#include "MapHandler.h"

#include "AbstractMapFileHandler.h"
#include "LegacyMapFileHandler.h"

#include "EDSMainMenu.h"

#include "PekaEDSMainView.h"

#include "MapMetadataComponent.h"

#include "GamePaths.h"

#include "Logger.h"

#include "EDSErrorDialog.h"

#include "EpisodeHandler.h"

class TilesetView;
class MapView;
class MiniMapView;

class PekaEDSMainView;

/*
	This is the main window and pretty much the main class of this application.

	It is responsible for loading maps and notifying all the other components of changes.
	Any component that wants to be notified of a map, tileset/background image and/or sprite list change needs to connect to one of this classes signals.
*/
class PekaEDSWindow : public QMainWindow {
	Q_OBJECT
		
public:
	PekaEDSWindow();

public slots:
	void changeTitle(std::string_view newTitle);

	void missingTileset(std::string_view filename);
	void missingBackground(std::string_view filename);

	void spriteLoadingErrors(const std::vector<QString>* errorList);

	// This slot gets called when anything about the map is changed. Tile/sprite placed, sprite added to list, tileset file changed, etc.
	void mapChanged();

signals:
	// Signals to be used by components. They must be connect in the components class.
	void tilesetImageLoaded(const QImage* newTileset);
	void tilesetImageChanged(const QImage* newTileset);
	void backgroundImageChanged(const QImage* newBackground);

	void mapMetadataChanged(const PK2::MapBaseMetadata* metadata);
	void mapDataChanged(const PK2::MapData& data);

private slots:
	void loggedMessage(const std::string& message);

private:
	void setupUI();
	void setupSlots();

	void showLoadMapDialog();
	void loadMap(std::string_view filename);

	void setMainView(PekaEDSMainView* newView);

private:
	inline static constexpr std::string_view TITLE_STRING = "PekaEDS";

	GamePaths gamePaths;

	MapHandler mapHandler;
	EpisodeHandler episodeHandler;

	// TODO Remove these?
	AbstractMapFileHandler* mapFileHandler = nullptr;
	LegacyMapFileHandler legacyMapFileHandler;
	
	EDSMainMenu* mainMenu = nullptr;
	
	PekaEDSMainView* mainView = nullptr;
	PekaEDSMainView* legacyMainView = nullptr;

	// TODO Remove these?
	TilesetView* tilesetView = nullptr;
	MapView* mapView = nullptr;
	MiniMapView* miniMapView = nullptr;
	MapMetadataComponent* metadataComponent = nullptr;

	EDS::Logger logger;
	EDSErrorDialog* errorDialog = nullptr;
};

