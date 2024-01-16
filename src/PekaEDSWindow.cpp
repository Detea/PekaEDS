#include "PekaEDSWindow.h"
#include "Version.h"

#include "ExpectedMapFormatException.h"

#include "MapMetadataView.h"

#include <filesystem>

#include <qboxlayout.h>
#include <qfiledialog.h>
#include <qmessagebox.h>

PekaEDSWindow::PekaEDSWindow() {
	setupUI();
	setupSlots();

	// TODO Set default map handler via settings?
	mapFileHandler = &legacyMapFileHandler;

	// TODO Again, development only
	// TODO Don't put these in this class, create an EDSMain or something?
	gamePaths.setBasePath(R"(D:\Pekka Kana 2)");

	// TODO TESTING ONLY!!!!!
	episodeHandler.setEpisodeFolder("rooster island 1");

	mapHandler.setGamePaths(&gamePaths);
	mapHandler.setEpisodeHandler(&episodeHandler);

	//loadMap(R"(D:\Pekka Kana 2\episodes\rooster island 1\level001.map)");

	changeTitle("test");

	//dumpObjectTree();
}

void PekaEDSWindow::setupUI() {
	mainMenu = new EDSMainMenu;

	errorDialog = new EDSErrorDialog(this);

	tilesetView = new TilesetView(this);
	mapView = new MapView(this);
	miniMapView = new MiniMapView(this);

	metadataComponent = new MapMetadataView(this);

	legacyMainView = new PekaEDSMainView(this);

	legacyMainView->setTilesetView(tilesetView);
	legacyMainView->setMapView(mapView);
	legacyMainView->setMiniMapView(miniMapView);
	legacyMainView->setMapMetadataComponent(metadataComponent);
	legacyMainView->create();

	setDockNestingEnabled(true);

	setMenuBar(mainMenu);
	setMainView(legacyMainView);

	// TODO For development only
	setMinimumSize(1280, 720);	
}

void PekaEDSWindow::setupSlots() {
	connect(mainMenu->getFileLoadAction(), &QAction::triggered, this, &PekaEDSWindow::showLoadMapDialog);

	connect(&mapHandler, &MapHandler::tilesetImageLoaded, this, &PekaEDSWindow::tilesetImageLoaded);
	connect(&mapHandler, &MapHandler::tilesetChanged, this, &PekaEDSWindow::tilesetImageChanged);
	connect(&mapHandler, &MapHandler::backgroundChanged, this, &PekaEDSWindow::backgroundImageChanged);

	connect(mapView, &MapView::viewportSizeChanged, miniMapView, &MiniMapView::setViewportSize);

	connect(miniMapView, &MiniMapView::viewportXChanged, mapView, &MapView::setViewportX);
	connect(miniMapView, &MiniMapView::viewportYChanged, mapView, &MapView::setViewportY);
	
	connect(mapView, &MapView::viewportXChanged, miniMapView, &MiniMapView::setViewportX);
	connect(mapView, &MapView::viewportYChanged, miniMapView, &MiniMapView::setViewportY);

	connect(&mapHandler, &MapHandler::mapMetadataChanged, this, &PekaEDSWindow::mapMetadataChanged);
	connect(&mapHandler, &MapHandler::mapDataChanged, this, &PekaEDSWindow::mapDataChanged);

	connect(&mapHandler, &MapHandler::missingTileset, this, &PekaEDSWindow::missingTileset);
	connect(&mapHandler, &MapHandler::missingBackground, this, &PekaEDSWindow::missingBackground);

	connect(&mapHandler, &MapHandler::spriteLoadingErrors, this, &PekaEDSWindow::spriteLoadingErrors);

	connect(this, &PekaEDSWindow::mapMetadataChanged, metadataComponent, &MapMetadataComponent::mapMetadataChanged);
}

void PekaEDSWindow::mapChanged() {
	qDebug() << "Map has changed!";
}

void PekaEDSWindow::missingTileset(std::string_view filename) {
	QMessageBox::warning(mainView, "Unable to load tileset!", "Can't read tileset image: " + QString(mapHandler.getTileset().data()));
}

void PekaEDSWindow::missingBackground(std::string_view filename) {
	QMessageBox::warning(mainView, "Unable to load background!", "Can't read background image: " + QString(mapHandler.getBackground().data()));
}

void PekaEDSWindow::spriteLoadingErrors(const std::vector<QString>* errorList) {
	errorDialog->show(errorList);
}

void PekaEDSWindow::showLoadMapDialog() {
	// TODO Add different map formats
	QString file = QFileDialog::getOpenFileName(this, "Open a Pekka Kana 2 map file...", "", "Pekka Kana 2 map file (*.map)");

	qDebug() << "Load map!";
}

void PekaEDSWindow::loggedMessage(const std::string& message) {
	// TODO Add message to errorDialog
	// Show after the message has been added? Or collect the messages?
}

void PekaEDSWindow::setMainView(PekaEDSMainView* newView) {
	mainView = newView;

	setCentralWidget(mainView);
}

void PekaEDSWindow::loadMap(std::string_view filename) {
	try {
		mapHandler.loadFile(filename);

		changeTitle(filename);
	} catch (const std::ios::failure& e) {
		QMessageBox::warning(mainView, "Unable to load map!", "Can't read map file!");
	} catch (const ExpectedMapFormatException& e) {
		QMessageBox::warning(mainView, "Wrong format!", QString::fromStdString(e.what()));
	}
}

void PekaEDSWindow::changeTitle(std::string_view newTitle) {
	QString title = TITLE_STRING.data();
	/*
	title += " ";
	title += Version::major;
	title += ".";
	title += Version::minor;
	title += ".";
	title += Version::patch;
	title += " - ";
	*/
	title += " - ";
	title += newTitle.data();

	setWindowTitle(title);
}