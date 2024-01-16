#include "MapHandler.h"
#include <qdebug.h>

#include <qpainter.h>

#include "ExpectedMapFormatException.h"
#include "WrongSpriteFormatException.h"
#include "MissingImageException.h"

#include "GFXUtil.h"

#include <fstream>
#include <string>

#include <qfile.h>
#include <filesystem>

#include <qelapsedtimer.h>

#define SET_SPRITE_MISSING(sprite)	\
	sprite.image = &missingImagePixmap;	\
	sprite.frameX = 0;	\
	sprite.frameY = 0;	\
	sprite.frameWidth = 32;	\
	sprite.frameHeight = 32;

MapHandler::MapHandler() {
	createMissingImagePixmap();
}

void MapHandler::createMissingImagePixmap() {
	missingImage = QImage(":/res/missing.bmp");

	missingImagePixmap = QPixmap::fromImage(missingImage);
}

void MapHandler::loadFile(std::string_view filename) {
	// TODO Check what version the map is and use the correct map reader

	// TODO Clear spriteImages map or keep it?
	try {
		QElapsedTimer timer;
		timer.start();

		legacyMapFileHandler.read(filename, metadata, mapData);
		
		qDebug() << "Loaded map in:" << timer.elapsed() << "msecs";

		bool loadedBackground = loadBackground(metadata.backgroundFile);
		if (!loadedBackground) {
			// Using signals instead of exceptions for this, because the code should continue, even if the file is missing.
			// Because then the background image gets set to the missing image and a MessageBox is displayed.
			emit missingBackground(metadata.backgroundFile);
		}

		bool loadedTileset = loadTileset(metadata.tilesetFile);
		if (!loadedTileset) {
			emit missingTileset(metadata.tilesetFile);
		}

		adjustPaletteAndColors();

		std::vector<QString> errorList;
		loadSpriteList(&errorList);

		if (!errorList.empty()) {
			emit spriteLoadingErrors(&errorList);
		}

		emit mapMetadataChanged(&metadata);
		emit mapDataChanged(mapData);
	} catch (const std::ios::failure& e) {
		throw;
	} catch (const ExpectedMapFormatException& e) {
		throw;
	}
}

// TODO Check if this is already loaded instead of reloading it every time?
bool MapHandler::loadTileset(std::string_view filename) {
	std::string file = gamePaths->getTilesetFile(filename);

	if (episodeHandler->hasEpisodeLoaded()) {
		std::string fileInEpisodeFolder = gamePaths->getTilesetFileEpisode(episodeHandler->episodePath(), filename);

		// Using std::filesystem here instead of QFile::exists because the latter is apparently case sensitive, which we don't want under Windows.
		if (std::filesystem::exists(std::filesystem::path(fileInEpisodeFolder))) {
			file = fileInEpisodeFolder;
		}
	}

	bool imageLoaded = tilesetImage.load(file.c_str());

	// TODO Check if image is in the correct format and size?
	if (!imageLoaded) {
		tilesetImage = missingImage;
	}

	emit tilesetImageLoaded(&tilesetImage);

	return imageLoaded;
}

// TODO Check if this is already loaded instead of reloading it every time?
bool MapHandler::loadBackground(std::string_view filename) {
	std::string file = gamePaths->getBackgroundFile(filename);

	if (episodeHandler->hasEpisodeLoaded()) {
		std::string fileInEpisodeFolder = gamePaths->getBackgroundFileEpisode(episodeHandler->episodePath(), filename);

		// Using std::filesystem here instead of QFile::exists because the latter is apparently case sensitive, which we don't want under Windows.
		if (std::filesystem::exists(std::filesystem::path(fileInEpisodeFolder))) {
			file = fileInEpisodeFolder;
		}
	}

	// TODO Send a signal and don't draw the background when it's missing???
	bool imageLoaded = backgroundImage.load(file.c_str());

	// TODO Check if image is in the correct format and size?
	if (!imageLoaded) {
		backgroundImage = missingImage;
	}

	emit backgroundChanged(&backgroundImage);

	return imageLoaded;
}

void MapHandler::adjustPaletteAndColors() {
	tilesetImage.setColorTable(backgroundImage.colorTable());

	// Make the last color in the palette transparent
	const QRgb alphaColor = tilesetImage.colorTable()[255];
	tilesetImage.setAlphaChannel(tilesetImage.createMaskFromColor(alphaColor, Qt::MaskOutColor));

	emit tilesetChanged(&tilesetImage);
}

std::string_view MapHandler::getBackground() {
	return metadata.backgroundFile;
}

std::string_view MapHandler::getTileset() {
	return metadata.tilesetFile;
}

void MapHandler::setTileAt(int x, int y, int tile, int layer) {
	if (layer >= 0 && layer < mapData.tileLayers.size()) {
		mapData.tileLayers[layer][x + mapWidth * static_cast<std::vector<int, std::allocator<int>>::size_type>(y)] = tile; // Isn't C++ beautiful?
	}
}

/*
	TODO What to do when a new level is loaded? Keep the existing spriteImageMap or clear it?
*/
void MapHandler::loadSpriteList(std::vector<QString>* errorList) {
	spriteImages.clear();

	mapData.spriteObjectList.clear();
	mapData.spriteObjectList.reserve(mapData.spriteFiles.size());

	PK2::SpriteBase tmpSprite;
	for (const std::string& filename : mapData.spriteFiles) {
		std::string spritePath = gamePaths->getSpritePath().data() + filename;

		bool loadedFromEpisodeFolder = false;
		if (episodeHandler->hasEpisodeLoaded()) {
			std::string spriteFileInEpisodeFolder = gamePaths->getEpisodesFolderPath().data() + filename;

			if (std::filesystem::exists(std::filesystem::path(spriteFileInEpisodeFolder))) {
				spritePath = spriteFileInEpisodeFolder;

				loadedFromEpisodeFolder = true;
			}
		}

		try {
			// TODO Handle different sprite formats
			// Have a spriteReader pointer AbstractSpriteReader* spriteReader that reads sprites
			// All sprite readers must throw WrongSpriteFormatException and ifstream::failure
			legacySpriteReader.read(spritePath, &tmpSprite);

			std::string imageFilePath = gamePaths->getSpritePath().data() + tmpSprite.imageFile;

			// Only load the image file from the episode folder if the sprite file exists in the episode folder, that's how the game does it
			if (loadedFromEpisodeFolder) {
				std::string imageFileInEpisodeFolder = gamePaths->getEpisodesFolderPath().data() + tmpSprite.imageFile;

				if (std::filesystem::exists(std::filesystem::path(imageFileInEpisodeFolder))) {
					imageFilePath = imageFileInEpisodeFolder;
				}
			}

			QImage image;
			if (!image.load(imageFilePath.c_str())) {
				// TODO Clean this up... gross
				QString errorMessage("Unable to load image file: \"");
				errorMessage += tmpSprite.imageFile;
				errorMessage += "\"";
				errorMessage += " for sprite file: \"";
				errorMessage += filename;
				errorMessage += "\"";
				errorList->push_back(errorMessage);

				SET_SPRITE_MISSING(tmpSprite)
			} else {
				std::string imageFileWithColor = tmpSprite.imageFile + std::to_string(tmpSprite.color);

				// Check if the sprites image has not been loaded already
				if (spriteImages.count(imageFileWithColor) == 0) {
					if (tmpSprite.color != 255) {
						GFXUtil::recolorSprite(&image, tmpSprite.color);
					}

					image.setColorTable(backgroundImage.colorTable());

					GFXUtil::makeTransparent(&image);

					spriteImages[imageFileWithColor] = QPixmap::fromImage(image);
				}

				tmpSprite.image = &spriteImages[imageFileWithColor];
			}

			mapData.spriteObjectList.push_back(tmpSprite);
		} catch (const WrongSpriteFormatException& e) {
			QString errorMessage(e.file().data());
			errorMessage += ": ";
			errorMessage += e.what();

			errorList->push_back(errorMessage);
		} catch (const std::ifstream::failure& e) {
			QString errorMessage("Unable to load sprite file: \"");
			errorMessage += filename;
			errorMessage += "\"";

			errorList->push_back(errorMessage);

			SET_SPRITE_MISSING(tmpSprite)

			mapData.spriteObjectList.push_back(tmpSprite);
		}
	}
}

void MapHandler::setGamePaths(const GamePaths* paths) {
	gamePaths = paths;
}

void MapHandler::setEpisodeHandler(const EpisodeHandler* manager) {
	episodeHandler = manager;
}