#pragma once

#include <qmainwindow.h>
#include <qpointer.h>

#include <string>
#include <string_view>
#include <vector>

#include "RegionView.h"
#include "MiniMapView.h"
#include "TilesetView.h"

#include "EDSToolBar.h"

#include "PK2MapBaseMetadata.h"
#include "PK2MapData.h"

#include "MapHandler.h"

#include "LegacySpriteFileReader.h"

#include "EDSMainMenu.h"

#include "PekaEDSMainView.h"
#include "MapMetadataView.h"
#include "RegionMetadataView.h"

#include "GamePaths.h"

#include "Logger.h"

#include "EDSErrorDialog.h"

#include "SpriteView.h"

#include "ImageCache.h"
#include "RegionsListView.h"

#include "Profile.h"

#include "Tool.h"
#include "BrushTool.h"
#include "SelectionTool.h"

#include "NewRegionDialog.h"

#include "Settings.h"
#include "SetGamePathView.h"

#include "MapPositionDialog.h"

/*
	This is the main window and pretty much the main class of this application.

	It is responsible for loading maps and notifying all the other components of changes.
	Any component that wants to be notified of a map, tileset/background image and/or sprite list change needs to connect to one of this classes signals.
*/
// TODO [CLEAN UP] Remove all unused methods and members
class PekaEDSWindow : public QMainWindow {
	Q_OBJECT
		
public:
	PekaEDSWindow();

public slots:
	void changeTitle(const QString& newTitle);

	// This slot gets called when anything about the map is changed. Tile/sprite placed, sprite added to list, tileset file changed, etc.
	void mapChanged();

    void tileSelectionChanged(const std::vector<int>& newSelection, int selectionWidth, int selectionHeight);
    void spriteSelected(int spriteNumber); // Used when a sprite is selected from the SpriteView
    // For selecting from the map?
    //void spriteSelectionChanged(const std::vector<int>& spriteNumbers, int selecitonWidth, int selectionHeight);


signals:
	// Signals to be used by components. They must be connect in the components class.
	void tilesetImageLoaded(const QImage* newTileset);
	void tilesetImageChanged(const QImage* newTileset);
	void backgroundImageChanged(const QImage* newBackground);

	void mapMetadataChanged(const PK2::MapBaseMetadata* metadata);
	void mapDataChanged(const PK2::MapData& data);

    void selectedRegionChanged(const Region& region);
    void selectedRegionIndexChanged(int selectedIndex);

    void profileLoaded(const Profile* profile);

    void selectedToolChanged(Tool* newSelectedTool);
    void selectedSecondaryToolChanged(Tool* newSelectedTool);

    void regionTilesetChanged(const QString& newTileset);
    void regionTilesetBackgroundChanged(const QString& newTileset);
    void regionBackgroundChanged(const QString& newBackground);

    void saveMapMetadata(PK2::MapBaseMetadata& metadata);
    void saveRegions(std::vector<Region>& regionList);

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
	void loggedMessage(const std::string& message);

    void regionChanged(int selectedRegionNumber);
    void regionListUpdated();
    void regionsLoaded(const std::vector<Region>* newRegionList);

    void triggerMapSave();
    void triggerMapSaveAs();
    void createNewMap();

private:
	void setupUI();
	void setupSlots();
    void miscSetup();

    void setLoadedFile(const QString& filename);

    void loadSettings();

    void showMainView();

	void showLoadMapDialog();
	void loadMap(const QString& filename);
    void saveMap(const QString& filePath);

    void setMainView(class PekaEDSMainView* newView);

    void showSetGamePathView();
    void setupDependencies();
    void gamePathSet(const QString& newPath);

private:
	inline static constexpr std::string_view TITLE_STRING = "PekaEDS";

    inline static const QString mapFilterString = "Pekka Kana 2 map file (*.map)";
    inline static const char* SETTINGS_FILE = "settings.eds";

	GamePaths gamePaths;

	LegacySpriteFileReader legacySpriteReader;

	MapHandler mapHandler;

	EDSMainMenu* mainMenu = nullptr;
	
	PekaEDSMainView* mainView = nullptr;

	EDSToolBar* toolBar = nullptr;

	// TODO Remove these?
	TilesetView* tilesetView = nullptr;
	RegionView* regionView = nullptr;
	MiniMapView* miniMapView = nullptr;
    MapMetadataView* metadataView = nullptr;

	SpriteView* spriteView = nullptr;

    RegionListView* regionListView = nullptr;
    RegionMetadataView* regionMetadataView = nullptr;
    NewRegionDialog* newRegionDialog = nullptr;

    SetGamePathView* setGamePathView = nullptr;

    MapPositionDialog* mapPositionDialog = nullptr;

    ImageCache imageCache;

    Profile profile;

	EDS::Logger logger;
	EDSErrorDialog* errorDialog = nullptr;

    Tool* selectedTool = nullptr;
    BrushTool brushTool;
    SelectionTool selectionTool;

    QString loadedFile;

    Settings settings;
};

