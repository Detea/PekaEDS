#include "PekaEDSWindow.h"

#include "MapMetadataView.h"

#include <qboxlayout.h>
#include <qfiledialog.h>
#include <qmessagebox.h>
#include "Tool.h"

PekaEDSWindow::PekaEDSWindow() {
    setMinimumSize(1280, 720);

    loadSettings();

	//dumpObjectTree();
}

void PekaEDSWindow::showMainView() {
    setupUI();
    setupSlots();

    setupDependencies();
    miscSetup();

    setMenuBar(mainMenu);
    setMainView(mainView);
    addToolBar(toolBar);

    toolBar->actnHighlightSprites->setChecked(settings.getHighlightSprites());
    toolBar->actnShowSpriteSize->setChecked(settings.getShowSpriteSize());
}

void PekaEDSWindow::showSetGamePathView() {
    setGamePathView = new SetGamePathView(this);
    connect(setGamePathView, &SetGamePathView::pathSet, this, &PekaEDSWindow::gamePathSet);

    setCentralWidget(setGamePathView);
}

void PekaEDSWindow::gamePathSet(const QString& newGamePath) {
    gamePaths.setBasePath(newGamePath);

    settings.setGameDirectory(newGamePath);

    showMainView();
}

void PekaEDSWindow::setupDependencies() {
    imageCache.setGamePaths(&gamePaths);

    mapHandler.setImageCache(&imageCache);
    mapHandler.setGamePaths(&gamePaths);

    spriteView->setGamePaths(&gamePaths);
    spriteView->setLegacySpriteReader(&legacySpriteReader);

    spriteView->setImageCache(imageCache);
    regionView->setImageCache(imageCache);
    miniMapView->setImageCache(imageCache);
    tilesetView->setImageCache(imageCache);
    ColorPreviewWidget::setImageCache(imageCache);

    mapPositionDialog->setGamePaths(gamePaths);
    metadataView->setMapPositionDialog(mapPositionDialog);

    RegionMetadataView::setGamePaths(&gamePaths);

    Tool::setImageCache(imageCache);
}

// TODO [CLEAN UP] Clean this up)
void PekaEDSWindow::miscSetup() {
    if (!profile.load(QCoreApplication::applicationDirPath() + "/profile.json")) {
        qDebug() << "Unable to load profile, setting defaults!";

        profile.setDefaults();
    }

    emit profileLoaded(&profile);

    selectedTool = &brushTool;
    emit selectedToolChanged(selectedTool);

    // TODO [CLEAN UP?] Not sure about this, but it'll do
    emit selectedSecondaryToolChanged(&selectionTool);

    // TODO Do this here?
    if (!QFile(gamePaths.getPK2StuffFile()).exists()) {
        QMessageBox::warning(this, "Unable to find pk2stuff.bmp", "Can't find pk2stuff.bmp!");
    }
}

void PekaEDSWindow::loadSettings() {
    if (QFile::exists(SETTINGS_FILE)) {
        if (!settings.load(SETTINGS_FILE)) {
            showSetGamePathView();
        } else {
            if (QFile::exists(settings.getGameDirectory())) {
                gamePaths.setBasePath(settings.getGameDirectory());

                showMainView();
            } else {
                showSetGamePathView();
            }
        }
    } else {
        showSetGamePathView();
    }
}

void PekaEDSWindow::setupUI() {
	mainMenu = new EDSMainMenu;

	errorDialog = new EDSErrorDialog(this);

	tilesetView = new TilesetView(this);
    regionView = new RegionView(this);
	miniMapView = new MiniMapView(this);

    metadataView = new MapMetadataView(this, gamePaths.getPK2StuffFile());

	spriteView = new SpriteView(this);

	mainView = new PekaEDSMainView(this);
	toolBar = new EDSToolBar(this);

    regionListView = new RegionListView(this);
    regionMetadataView = new RegionMetadataView(this);
    newRegionDialog = new NewRegionDialog(this);

    mapPositionDialog = new MapPositionDialog(this);

    regionListView->setNewRegionDialog(newRegionDialog);

    mainView->setTilesetView(tilesetView)
        .setMapView(regionView)
        .setMiniMapView(miniMapView)
        .setMapMetadataView(metadataView)
        .setSpriteView(spriteView)
        .setRegionView(regionListView)
        .setRegionMetadataView(regionMetadataView)
        .create();

	setDockNestingEnabled(true);
}

void PekaEDSWindow::closeEvent(QCloseEvent *event) {
    // TODO Save reminder
    settings.setHighlightSprites(toolBar->actnHighlightSprites->isChecked());
    settings.setShowSpriteSize(toolBar->actnShowSpriteSize->isChecked());
    settings.save(SETTINGS_FILE);
}

void PekaEDSWindow::setupSlots() {
    connect(mainMenu->mFileLoad, &QAction::triggered, this, &PekaEDSWindow::showLoadMapDialog);
    connect(mainMenu->mFileSave, &QAction::triggered, this, &PekaEDSWindow::triggerMapSave);
    connect(mainMenu->mFileSaveAs, &QAction::triggered, this, &PekaEDSWindow::triggerMapSaveAs);

	connect(toolBar->actnLoad, &QAction::triggered, this, &PekaEDSWindow::showLoadMapDialog);
    connect(toolBar->actnSave, &QAction::triggered, this, &PekaEDSWindow::triggerMapSave);
    connect(toolBar->actnSaveAs, &QAction::triggered, this, &PekaEDSWindow::triggerMapSaveAs);

    connect(toolBar->actnHighlightSprites, &QAction::toggled, regionView, &RegionView::setHighlightSprites);
    connect(toolBar->actnShowSpriteSize, &QAction::toggled, regionView, &RegionView::setShowSpriteSize);
    connect(toolBar->actnShowForegroundSprites, &QAction::toggled, regionView, &RegionView::setShowForegroundSprites);
    connect(toolBar->actnShowBackgroundSprites, &QAction::toggled, regionView, &RegionView::setShowBackgroundSprites);

    connect(toolBar->cbLayers, &QComboBox::currentIndexChanged, regionView, &RegionView::setSelectedLayer);

    connect(&mapHandler, &MapHandler::mapMetadataChanged, regionView, &RegionView::mapMetadataChanged);

    connect(miniMapView, &MiniMapView::viewportXChanged, regionView, &RegionView::setViewportX);
    connect(miniMapView, &MiniMapView::viewportYChanged, regionView, &RegionView::setViewportY);

    connect(regionListView, &RegionListView::regionAdded, miniMapView, &MiniMapView::regionAdded);
    connect(&mapHandler, &MapHandler::mapRegionsLoaded, miniMapView, &MiniMapView::mapRegionsLoaded);
    connect(&mapHandler, &MapHandler::regionForegroundTilesetFileChanged, miniMapView, &MiniMapView::foregroundTilesetChanged);
    connect(&mapHandler, &MapHandler::regionBackgroundTilesetFileChanged, miniMapView, &MiniMapView::backgroundTilesetChanged);
    connect(&mapHandler, &MapHandler::regionBackgroundFileChanged, miniMapView, &MiniMapView::backgroundImageChanged);
    connect(&mapHandler, &MapHandler::updateMap, miniMapView, &MiniMapView::updateMap);

	connect(&mapHandler, &MapHandler::mapMetadataChanged, this, &PekaEDSWindow::mapMetadataChanged);
	connect(&mapHandler, &MapHandler::mapDataChanged, this, &PekaEDSWindow::mapDataChanged);

    connect(&mapHandler, &MapHandler::errorsEncountered, errorDialog, &EDSErrorDialog::showErrors);

	connect(&mapHandler, &MapHandler::spriteListLoaded, spriteView, &SpriteView::setSpriteData);

    connect(spriteView, &SpriteView::spriteReplaced, &mapHandler, &MapHandler::spriteReplaced);
    connect(spriteView, &SpriteView::spriteSetAsPlayer, &mapHandler, &MapHandler::spriteSetAsPlayer);
    connect(spriteView, &SpriteView::spriteRemoved, &mapHandler, &MapHandler::spriteRemoved);
    connect(spriteView, &SpriteView::spritesAdded, &mapHandler, &MapHandler::spritesAdded);

    connect(&mapHandler, &MapHandler::mapChanged, regionView, &RegionView::mapChanged);

    connect(this, &PekaEDSWindow::mapMetadataChanged, metadataView, &MapMetadataView::mapMetadataChanged);

    connect(&mapHandler, &MapHandler::mapRegionsLoaded, regionListView, &RegionListView::regionsLoaded);
    connect(&mapHandler, &MapHandler::mapRegionsLoaded, tilesetView, &TilesetView::regionsLoaded);

    connect(regionMetadataView, &RegionMetadataView::tilesetChanged, &mapHandler, &MapHandler::regionTilesetChanged);

    // TODO [IMPROVEMENT] Create a MapHandler::tilesetChanged signal with no parameters and call RegionView::update directly?
    // Only need these to update RegionView when the tilesets change
    connect(&mapHandler, &MapHandler::regionForegroundTilesetFileChanged, regionView, &RegionView::tilesetChanged);
    connect(&mapHandler, &MapHandler::regionBackgroundTilesetFileChanged, regionView, &RegionView::tilesetChanged);

    connect(regionMetadataView, &RegionMetadataView::tilesetBackgroundChanged, &mapHandler, &MapHandler::regionBackgroundTilesetChanged);
    connect(regionMetadataView, &RegionMetadataView::backgroundTilesetCleared, &mapHandler, &MapHandler::regionBackgroundTilesetFileCleared);
    connect(regionMetadataView, &RegionMetadataView::backgroundChanged, &mapHandler, &MapHandler::regionBackgroundChanged);

    connect(this, &PekaEDSWindow::regionTilesetChanged, miniMapView, &MiniMapView::foregroundTilesetChanged);

    connect(regionListView, &RegionListView::selectedRegionChanged, this, &PekaEDSWindow::regionChanged);
    connect(this, &PekaEDSWindow::selectedRegionChanged, regionMetadataView, &RegionMetadataView::selectedRegionChanged);
    connect(this, &PekaEDSWindow::selectedRegionIndexChanged, tilesetView, &TilesetView::selectedRegionIndexChanged);

    connect(regionListView, &RegionListView::regionAdded, &mapHandler, &MapHandler::regionAdded);
    connect(regionListView, &RegionListView::regionRemoved, &mapHandler, &MapHandler::regionRemoved);

    connect(this, &PekaEDSWindow::profileLoaded, &mapHandler, &MapHandler::profileChanged);
    connect(this, &PekaEDSWindow::profileLoaded, regionMetadataView, &RegionMetadataView::profileLoaded);
    connect(this, &PekaEDSWindow::profileLoaded, newRegionDialog, &NewRegionDialog::profileLoaded);
    connect(this, &PekaEDSWindow::profileLoaded, metadataView, &MapMetadataView::profileLoaded);

    connect(&mapHandler, &MapHandler::mapRegionsLoaded, regionView, &RegionView::regionsLoaded);
    connect(regionListView, &RegionListView::selectedRegionChanged, regionView, &RegionView::selectedRegionChanged);
    connect(regionListView, &RegionListView::selectedRegionChanged, &mapHandler, &MapHandler::selectedRegionChanged);

    connect(&mapHandler, &MapHandler::mapRegionsLoaded, spriteView, &SpriteView::regionsLoaded);
    connect(&mapHandler, &MapHandler::mapRegionsLoaded, this, &PekaEDSWindow::regionsLoaded);

    connect(&mapHandler, &MapHandler::mapRegionsLoaded, miniMapView, &MiniMapView::mapRegionsLoaded);
    connect(this, &PekaEDSWindow::selectedRegionIndexChanged, miniMapView, &MiniMapView::selectedRegionChanged);
    connect(regionView, &RegionView::viewportSizeChanged, miniMapView, &MiniMapView::setViewportSize);
    connect(regionView, &RegionView::viewportXChanged, miniMapView, &MiniMapView::setViewportX);
    connect(regionView, &RegionView::viewportYChanged, miniMapView, &MiniMapView::setViewportY);

    // Connecting the signal from PekaEDSWindow instead of RegionListView because no region is selected when a level is loaded
    connect(this, &PekaEDSWindow::selectedRegionIndexChanged, spriteView, &SpriteView::selectedRegionChanged);

    connect(this, &PekaEDSWindow::selectedToolChanged, regionView, &RegionView::selectedToolChanged);
    connect(this, &PekaEDSWindow::selectedSecondaryToolChanged, regionView, &RegionView::selectedSecondaryToolChanged);

    connect(toolBar->cbLayers, &QComboBox::currentIndexChanged, &mapHandler, &MapHandler::selectedLayerChanged);
    connect(toolBar->cbLayers, &QComboBox::currentIndexChanged, tilesetView, &TilesetView::selectedLayerChanged);

    // NOTE IMPORTANT: tilePlaced and spritePlaced signals have to be connected for each tool!
    connect(&brushTool, &BrushTool::tilePlaced, &mapHandler, &MapHandler::tilePlaced);
    connect(&brushTool, &BrushTool::spritePlaced, &mapHandler, &MapHandler::spritePlaced);

    connect(&selectionTool, &SelectionTool::grabTiles, &mapHandler, &MapHandler::grabTiles);
    connect(&selectionTool, &SelectionTool::grabSprites, &mapHandler, &MapHandler::grabSprites);

    connect(spriteView, &SpriteView::selectionChanged, this, &PekaEDSWindow::spriteSelected);
    connect(tilesetView, &TilesetView::tileSelectionChanged, this, &PekaEDSWindow::tileSelectionChanged);

    // Save map
    connect(this, &PekaEDSWindow::saveMapMetadata, metadataView, &MapMetadataView::saveMapMetadata);
    connect(this, &PekaEDSWindow::saveMapMetadata, &mapHandler, &MapHandler::saveMapMetadata);
    connect(this, &PekaEDSWindow::saveRegions, &mapHandler, &MapHandler::saveRegions);
}

void PekaEDSWindow::mapChanged() {
	qDebug() << "Map has changed!";
}

void PekaEDSWindow::spriteSelected(int spriteNumber) {
    Tool::setSelectedSprite(spriteNumber);
    Tool::setMode(Tool::MODE::SPRITE);

    toolBar->cbMode->setCurrentIndex(1);
}

void PekaEDSWindow::tileSelectionChanged(const std::vector<int>& newSelection, int selectionWidth, int selectionHeight) {
    Tool::setSelectedTiles(newSelection, selectionWidth, selectionHeight);
    Tool::setMode(Tool::MODE::TILE);

    toolBar->cbMode->setCurrentIndex(0);
}

void PekaEDSWindow::showLoadMapDialog() {
    QString file = QFileDialog::getOpenFileName(this, "Open a Pekka Kana 2 map file...", gamePaths.episodesFolder(), mapFilterString);

    loadMap(file);
}

void PekaEDSWindow::loggedMessage(const std::string& message) {
	// TODO Add message to errorDialog
	// Show after the message has been added? Or collect the messages?
}

void PekaEDSWindow::setMainView(PekaEDSMainView* newView) {
	mainView = newView;

	setCentralWidget(mainView);
}

void PekaEDSWindow::createNewMap() {
    loadedFile = "";

    // TODO [IMPLEMENT] Reset everything
}

void PekaEDSWindow::loadMap(const QString& filepath) {
    if (!filepath.isEmpty()) {
        MapHandler::Result result = mapHandler.loadFile(filepath);

        if (result == MapHandler::Result::SUCCESS) {
            loadedFile = filepath;
            changeTitle(filepath);

            regionChanged(0);

            emit selectedRegionIndexChanged(0);

            // TODO [CLEAN UP] Create a switchLayer method? For shortcuts
            toolBar->setSelectedLayer(0);
            tilesetView->selectedLayerChanged(0);
            mapHandler.selectedLayerChanged(0);
        } else {
            QMessageBox::critical(this, "Error", mapHandler.getLastError());
        }
    }
}

void PekaEDSWindow::triggerMapSave() {
    QString filePath  = loadedFile;

    // When no map is loaded, ask the user for a file to save as
    if (loadedFile.isEmpty()) {
        filePath = QFileDialog::getSaveFileName(this, "Save map file...", "", mapFilterString);
    }

    if (!filePath.isEmpty()) {
        if (!filePath.endsWith(".map")) {
            filePath += ".map";
        }

        loadedFile = filePath;

        saveMap(filePath);
    }
}

void PekaEDSWindow::triggerMapSaveAs() {
    QString dirPath = gamePaths.episodesFolder();

    QString currentEpisode = gamePaths.currentEpisodeFolder();
    if (!currentEpisode.isEmpty()) {
        dirPath = currentEpisode;
    }

    QString filename = QFileDialog::getSaveFileName(this, "Save map as...", dirPath, mapFilterString);

    if (!filename.isEmpty()) {
        if (!filename.endsWith(".map")) filename += ".map";

        saveMap(filename);
        setLoadedFile(filename);
    }
}

void PekaEDSWindow::saveMap(const QString& filePath) {
    PK2::MapBaseMetadata metadata;
    std::vector<Region> regionList;

    emit saveMapMetadata(metadata);
    emit saveRegions(regionList);

    FileHandler fileHandler;

    if (fileHandler.saveMapVersion15(filePath, metadata, regionList) != FileHandler::SUCCESS) {
        QMessageBox::warning(this, "Unable to save map!", fileHandler.lastError());
    }
}

void PekaEDSWindow::setLoadedFile(const QString& filename) {
    loadedFile = filename;
    changeTitle(loadedFile);
}

void PekaEDSWindow::changeTitle(const QString& newTitle) {
	QString title = TITLE_STRING.data();
	title += " - ";
	title += newTitle.data();

	setWindowTitle(title);
}

void PekaEDSWindow::regionListUpdated() {
    emit selectedRegionChanged(mapHandler.getRegion(0));
}

void PekaEDSWindow::regionsLoaded(const std::vector<Region>* newRegionList) {
    Tool::setRegionList(newRegionList);
}

void PekaEDSWindow::regionChanged(int selectedRegionNumber) {
    const Region* region = &mapHandler.getRegion(selectedRegionNumber);

    Tool::setSelectedRegion(selectedRegionNumber);

    emit selectedRegionChanged(*region); // TODO Pass region number instead?
    emit selectedRegionIndexChanged(selectedRegionNumber);

    emit regionTilesetChanged(region->tilesetImageFile);
    emit regionTilesetBackgroundChanged(region->tilesetBackgroundImageFile);
    emit regionBackgroundChanged(region->backgroundImageFile);
}
