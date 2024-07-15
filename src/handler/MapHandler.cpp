#include "MapHandler.h"
#include <qdebug.h>

#include <qpainter.h>

#include "Region.h"
#include "PK2LegacyMapData.h"

#include <qmessagebox.h>

#include <qfile.h>

#include <qelapsedtimer.h>
#include "Tool.h"

#define CHECK_VERSION(given, expected) \
    given[0] == expected[0] &&\
    given[1] == expected[1] &&\
    given[2] == expected[2] &&\
    given[3] == expected[3]


#define SET_SPRITE_MISSING(sprite)	\
	sprite.frameX = 0;	\
	sprite.frameY = 0;	\
	sprite.frameWidth = 32;	\
	sprite.frameHeight = 32; \
    sprite.name = "(Missing)"; \
    sprite.imageFile = "missing";\
    sprite.color = 255;\

#define SPRITE_ERROR(spriteFilename, errorMessage) errorList.push_back("[" + spriteFilename + "] " + errorMessage);

MapHandler::MapHandler() {
	createMissingImagePixmap();
}

void MapHandler::createMissingImagePixmap() {
    bool loaded = missingImage.load(":/res/missing.bmp");

    // This should never trigger, but putting this here just in case
    Q_ASSERT(loaded);

	missingImagePixmap = QPixmap::fromImage(missingImage);
}

MapHandler::Result MapHandler::loadFile(const QString& filename) {
    QFile mapFile(filename);

    std::vector<QString> errorList;

    if (!mapFile.open(QIODevice::ReadOnly)) {
        lastError = "Can't open file for reading.";

        return Result::UNABLE_TO_READ_FILE;
    }

    char data[4];
    mapFile.read(data, 4);

    if (CHECK_VERSION(data, MAP_HEADER_13)) {
        PK2::LegacyMapData legacyMapData;

        if (fileHandler.readMapVersion13(filename, legacyMapData) == FileHandler::SUCCESS) {
            regions.clear();

            convertV13ToV15(legacyMapData);
        } else {
            lastError = fileHandler.lastError();

            return Result::WRONG_MAP_FORMAT;
        }
    } else if (CHECK_VERSION(data, MAP_HEADER_15)) {
        // Creating a temporary list in case reading the map fails, so that the currently loaded map just stays loaded
        std::vector<Region> tempRegionList;

        if (fileHandler.readMapVersion15(filename, metadata, tempRegionList) == FileHandler::SUCCESS) {
            regions = tempRegionList;
        } else {
            lastError = fileHandler.lastError();

            return Result::WRONG_MAP_FORMAT;
        }
    } else {
        lastError = "The provided file does not seem to be a Pekka Kana 2 map.";

        return Result::WRONG_MAP_FORMAT;
    }

    // At this point at least one region has to have been created
    if (regions.empty()) {
        lastError = "No regions found im map file.";

        return Result::FAILURE;
    }

    for (Region& region : regions) {
        QString bgFile = gamePaths->getBackgroundFile(region.backgroundImageFile);
        bool bgResult = imageCache->addBackground(bgFile);
        if (!bgResult) {
            region.missingBackground = true;

            errorList.push_back("[" + bgFile + "]:\n" + imageCache->error());
        }

        bool tilesetResult = imageCache->addTileset(gamePaths->getTilesetFile(region.tilesetImageFile), region.backgroundImageFile);
        if (!tilesetResult) {
            errorList.push_back(imageCache->error());
        }

        if (!region.tilesetBackgroundImageFile.isEmpty()) {
            bool result = imageCache->addTileset(gamePaths->getTilesetFile(region.tilesetBackgroundImageFile), region.backgroundImageFile);

            if (!result) {
                errorList.push_back(imageCache->error());
            }
        }
    }

    loadSpriteList(&errorList);

    // Calculate the sprites "placement" properties
    // This has to be done here because the sprites need to be loaded first and before sprites can be loaded the background has to be loaded
    for (const Region& region : regions) {
        const std::vector<int>& spriteLayer = region.spriteLayer;

        for (int i = 0; i < spriteLayer.size(); ++i) {
            if (spriteLayer[i] != 255 && spriteLayer[i] < spriteLayer.size()) {
                metadata.spriteObjects[spriteLayer[i]].placementAmount++;
            }
        }
    }

    if (!errorList.empty()) {
        emit errorsEncountered(errorList);
    }

    emit mapMetadataChanged(&metadata);
    emit mapRegionsLoaded(&regions);

    return Result::SUCCESS;
}

// TODO [CLEAN UP] It would be a lot cleaner to use a logger instead of passing a vector of strings, but this works and I want to get this project done. So maybe later.
void MapHandler::loadSpriteList(std::vector<QString>* errorList) {
    metadata.spriteObjects.clear();
    metadata.spriteObjects.reserve(metadata.spriteFilenames.size());

	PK2::SpriteBase tmpSprite;
    for (const QString& filename : metadata.spriteFilenames) {
        //QString spritePath = gamePaths->spriteFolder() + filename;
        QString spriteFile = gamePaths->getSprite(filename);

        tmpSprite.filename = filename;

        if (!QFile::exists(spriteFile) && filename.endsWith(".spr")) {
            spriteFile = gamePaths->getSprite(filename + "2");
            tmpSprite.filename += "2";
        }

        if (!QFile::exists(spriteFile)) {
            SET_SPRITE_MISSING(tmpSprite)
            imageCache->addMissingSprite(tmpSprite, regions.at(selectedRegion).backgroundImageFile);

            metadata.spriteObjects.push_back(tmpSprite);

            errorList->push_back("Unable to find sprite file: " + filename);
        } else {
            int result = -1;

            if (spriteFile.endsWith(".spr")) {
                result = fileHandler.readSpriteVersion13(spriteFile, tmpSprite);
            } else if (spriteFile.endsWith(".spr2")) {
                result = fileHandler.readSpriteVersion15(spriteFile, tmpSprite, *errorList);
            }

            if (result == FileHandler::SUCCESS) {
                // Cache this value, because it will be used everything a sprite image is requested from ImageCache!
                tmpSprite.imageFileWithColor = tmpSprite.imageFile + QString::number(tmpSprite.color);

                imageCache->addSprite(tmpSprite, gamePaths->getSpriteImage(tmpSprite), regions.at(selectedRegion).backgroundImageFile);

                metadata.spriteObjects.push_back(tmpSprite);
            } else if (result == FileHandler::ERROR_WRONG_VERSION) {
                // TODO Remove sprite from level?
                errorList->push_back(fileHandler.lastError());
            }
        }
	}

    if (!metadata.spriteObjects.empty() &&
		metadata.playerSpriteIndex >= 0 && 
        metadata.playerSpriteIndex < metadata.spriteObjects.size()) {
        metadata.spriteObjects[metadata.playerSpriteIndex].player = true;
	}

    // TODO [CLEAN UP?] Kinda gross to have this in here, ideally this would just catch the signal below, but that doesn't work with static classes.
    Tool::setSpriteList(&metadata.spriteObjects);

    emit spriteListLoaded(&metadata.spriteFilenames, &metadata.spriteObjects);
}

void MapHandler::convertV13ToV15(const PK2::LegacyMapData& legacyMapData) {
    Region region;

    region.width = 256;
    region.height = 224;

    region.scrolling = legacyMapData.scrollingType;
    region.weather = legacyMapData.weatherType;

    region.backgroundImageFile = legacyMapData.backgroundFile;
    region.tilesetImageFile = legacyMapData.tilesetFile;

    region.musicFile = legacyMapData.musicFile;

    region.backgroundLayer = legacyMapData.backgroundLayer;
    region.foregroundLayer = legacyMapData.foregroundLayer;
    region.spriteLayer = legacyMapData.spriteLayer;

    region.editorName = "Main";

    metadata.spriteFilenames = legacyMapData.spriteFilenames;

    metadata.name = legacyMapData.name;
    metadata.author = legacyMapData.author;
    metadata.time = legacyMapData.time;
    metadata.iconNumber = legacyMapData.iconNumber;
    metadata.mapX = legacyMapData.mapX;
    metadata.mapY = legacyMapData.mapY;
    metadata.levelNumber = legacyMapData.levelNumber;

    regions.push_back(region);
}

void MapHandler::spritesAdded(const QStringList& filePaths) {
    for (auto& s : filePaths) {
        metadata.spriteFilenames.push_back(s);

        // TODO [CLEAN UP] Clean this mess up!!!
        PK2::SpriteBase spr;
        if (s.endsWith(".spr")) {
            if (fileHandler.readSpriteVersion13(s, spr) == FileHandler::SUCCESS) {
                spr.imageFileWithColor = spr.imageFile + QString::number(spr.color);

                imageCache->addSprite(spr, gamePaths->getSpriteImage(spr), regions[0].backgroundImageFile);

                metadata.spriteObjects.push_back(spr);
            } else {
                // TODO Don't create MessageBoxes in this class, emit an error signal instead!
                QMessageBox::warning(nullptr, "Unable to add sprite!", fileHandler.lastError());
            }
        } else if (s.endsWith(".spr2")) {
            // TODO [CLEAN UP] Handle the errors
            std::vector<QString> errorList;
            if (fileHandler.readSpriteVersion15(s, spr, errorList) == FileHandler::SUCCESS) {
                spr.imageFileWithColor = spr.imageFile + QString::number(spr.color);

                imageCache->addSprite(spr, gamePaths->getSpriteImage(spr), regions[0].backgroundImageFile);

                metadata.spriteObjects.push_back(spr);
            } else {
                // TODO Don't create MessageBoxes in this class, emit an error signal instead!
                QMessageBox::warning(nullptr, "Unable to add sprite!", fileHandler.lastError());
            }
        }
    }

    emit saveReminderTriggered();
}

void MapHandler::spriteReplaced(int spriteListIndex, const QString& newSpriteFilePath) {
    Q_ASSERT(spriteListIndex < metadata.spriteObjects.size());
    Q_ASSERT(spriteListIndex < metadata.spriteFilenames.size());

    PK2::SpriteBase spr;
    int result = FileHandler::ERROR_FILE_NOT_FOUND;

    if (newSpriteFilePath.endsWith(".spr")) {
        result = fileHandler.readSpriteVersion13(newSpriteFilePath, spr);
    } else if (newSpriteFilePath.endsWith(".spr2")) {
        // TODO [CLEAN UP, FIX?] Should errors be emitted? If so should probably also do it for the 13 reader above!
        std::vector<QString> errorList;
        result = fileHandler.readSpriteVersion15(newSpriteFilePath, spr, errorList);
    }

    if (result == FileHandler::SUCCESS) {
        spr.imageFileWithColor = spr.imageFile + QString::number(spr.color);

        imageCache->addSprite(spr, gamePaths->getSpriteImage(spr), regions.at(selectedRegion).backgroundImageFile);

        metadata.spriteObjects[spriteListIndex] = spr;
        metadata.spriteFilenames[spriteListIndex] = newSpriteFilePath;

        emit saveReminderTriggered();
        emit mapChanged();
    }
}

void MapHandler::spriteSetAsPlayer(int newPlayerIndex) {
    if (metadata.spriteObjects[newPlayerIndex].type == 1 && !metadata.spriteObjects[newPlayerIndex].player) {
        for (auto& spr : metadata.spriteObjects) {
            if (spr.player) {
                spr.player = false;

                break;
            }
        }

        metadata.spriteObjects[newPlayerIndex].player = true;

        emit saveReminderTriggered();
    }
}

void MapHandler::spriteRemoved(int spriteListIndex) {
    metadata.spriteFilenames.erase(metadata.spriteFilenames.begin() + spriteListIndex);
    metadata.spriteObjects.erase(metadata.spriteObjects.begin() + spriteListIndex);

    for (int regionIndex = 0; regionIndex < regions.size(); ++regionIndex) {
        for (int i = 0; i < regions[regionIndex].width * regions[regionIndex].height; ++i) {
            if (regions[regionIndex].spriteLayer[i] == spriteListIndex) {
                regions[regionIndex].spriteLayer[i] = 255;
            } else if (regions[regionIndex].spriteLayer[i] > spriteListIndex) {
                --regions[regionIndex].spriteLayer[i];
            }
        }
    }

    emit mapChanged();
}

void MapHandler::regionAdded(const Region& addedRegion) {
    Q_ASSERT(profile != nullptr);

    Region region;

    if (!addedRegion.editorName.isEmpty()) {
        region.editorName = addedRegion.editorName;
    } else {
        region.editorName = "Region #" + QString::number(regions.size() + 1);
    }

    region.tilesetImageFile = addedRegion.tilesetImageFile;
    region.tilesetBackgroundImageFile = addedRegion.tilesetBackgroundImageFile;
    region.backgroundImageFile = addedRegion.backgroundImageFile;

    region.musicFile = addedRegion.musicFile;

    region.width = addedRegion.width;
    region.height = addedRegion.height;

    region.weather = addedRegion.weather;
    region.scrolling = addedRegion.scrolling;

    region.fireColor1 = addedRegion.fireColor1;
    region.fireColor2 = addedRegion.fireColor2;
    region.splashColor = addedRegion.splashColor;

    imageCache->addBackground(gamePaths->getBackgroundFile(region.backgroundImageFile));
    imageCache->addTileset(gamePaths->getTilesetFile(region.tilesetImageFile), region.backgroundImageFile);

    if (!region.tilesetBackgroundImageFile.isEmpty()) {
        imageCache->addTileset(gamePaths->getTilesetFile(region.tilesetBackgroundImageFile), region.backgroundImageFile);
    }

    std::vector<QString> errorList;
    std::vector<PK2::SpriteBase>& spriteList = metadata.spriteObjects;
    for (int i = 0; i < spriteList.size(); ++i) {
        PK2::SpriteBase& spr = spriteList[i];

        const QString imagePath = gamePaths->getSpriteImage(spr);

        if (!imageCache->hasSpriteImage(spr, region.backgroundImageFile)) {
            if (!imageCache->addSprite(spr, imagePath, region.backgroundImageFile)) {
                SPRITE_ERROR(imagePath, "Unable to find file!")
            }
        }
    }

    if (!errorList.empty()) {
        emit errorsEncountered(errorList);
    }

    region.foregroundLayer.resize(region.width * region.height, 255);
    region.backgroundLayer.resize(region.width * region.height, 255);

    region.spriteLayer.resize(region.width * region.height, 255);

    regions.push_back(region);

    //emit regionAdded(regions.back());
    // TODO [CLEAN UP] Don't use this?
    emit regionListUpdated();
}

void MapHandler::regionRemoved(int regionIndex) {
    Q_ASSERT(regionIndex >= 0 && regionIndex < regions.size());

    regions.erase(regions.begin() + regionIndex);
}

const Region& MapHandler::getRegion(int regionNumber) {
    Q_ASSERT(regionNumber >= 0 && regionNumber < regions.size());

    return regions[regionNumber];
}

void MapHandler::tilePlaced(int x, int y, int selectionWidth, int selectionHeight, const std::vector<int>& selectedTiles) {
    x /= 32;
    y /= 32;

    Region& region = regions[selectedRegion];
    if (x < 0) x = 0;
    if (y < 0) y = 0;
    if (x + selectionWidth > region.width) x = region.width - selectionWidth;
    if (y + selectionHeight > region.height) y = region.height - selectionHeight;

    std::vector<int>& layer = selectedLayer == Layer::BACKGROUND ? region.backgroundLayer : region.foregroundLayer;

    int i = 0;
    for (int ty = y; ty < y + selectionHeight; ++ty) {
        for (int tx = x; tx < x + selectionWidth; ++tx) {
            layer[tx + ty * region.width] = selectedTiles[i];

            ++i;
        }
    }

    emit updateMap(x, y, selectionWidth, selectionHeight, selectedLayer);
    // TODO Undo
    // TODO Save reminder
}

void MapHandler::spritePlaced(int x, int y, int selectionWidth, int selectionHeight, const std::vector<int>& selectedSprites) {
    x /= 32;
    y /= 32;

    Region& region = regions[selectedRegion];
    if (x < 0) x = 0;
    if (y < 0) y = 0;
    if (x + selectionWidth > region.width) x = region.width - selectionWidth;
    if (y + selectionHeight > region.height) y = region.height - selectionHeight;

    int i = 0;
    for (int ty = y; ty < y + selectionHeight; ++ty) {
        for (int tx = x; tx < x + selectionWidth; ++tx) {
            if (region.spriteLayer[tx + ty * region.width] != selectedSprites[i]) {
                ++metadata.spriteObjects[selectedSprites[i]].placementAmount;
            }

            region.spriteLayer[tx + ty * region.width] = selectedSprites[i];

            ++i;
        }
    }

    // TODO undo
    // TODO save reminder
}

void MapHandler::grabTiles(const QRect& selectionRect) {
    int startX = selectionRect.x() / 32;
    int startY = selectionRect.y() / 32;
    int width = selectionRect.width() / 32;
    int height = selectionRect.height() / 32;

    Region& region = regions[selectedRegion];
    std::vector<int>& layer = selectedLayer == Layer::BACKGROUND ? region.backgroundLayer : region.foregroundLayer;

    // TODO [CLEAN UP] Not ideal to create a new vector here, but it doesn't happen that often so it should be fine
    std::vector<int> tiles;
    for (int ty = startY; ty < startY + height; ++ty) {
        for (int tx = startX; tx < startX + width; ++tx) {
            tiles.push_back(layer[tx + ty * region.width]);
        }
    }

    Tool::setSelectedTiles(tiles, width, height);
}

void MapHandler::grabSprites(const QRect& selectionRect) {
    int startX = selectionRect.x() / 32;
    int startY = selectionRect.y() / 32;
    int width = selectionRect.width() / 32;
    int height = selectionRect.height() / 32;

    std::vector<int> sprites;
    Region& region = regions[selectedRegion];

    for (int ty = startY; ty < startY + height; ++ty) {
        for (int tx = startX; tx < startX + width; ++tx) {
             sprites.push_back(region.spriteLayer[tx + ty * region.width]);
        }
    }

    Tool::setSelectedSprites(sprites, width, height);
}


void MapHandler::saveMapMetadata(PK2::MapBaseMetadata& otherMetadata) {
    otherMetadata = metadata;
}

void MapHandler::saveRegions(std::vector<Region>& otherRegionList) {
    otherRegionList = regions;
}

void MapHandler::regionTilesetChanged(const QString& newTileset) {
    Q_ASSERT(selectedRegion >= 0 && selectedRegion < regions.size());

    imageCache->addTileset(gamePaths->getTilesetFile(newTileset), regions[selectedRegion].backgroundImageFile);
    regions[selectedRegion].tilesetImageFile = newTileset;

    emit regionForegroundTilesetFileChanged(newTileset);
}

void MapHandler::regionBackgroundChanged(const QString& newBackground) {
    Q_ASSERT(selectedRegion >= 0 && selectedRegion < regions.size());

    imageCache->changeBackground(regions[selectedRegion].backgroundImageFile, gamePaths->getBackgroundFile(newBackground));
    regions[selectedRegion].backgroundImageFile = newBackground;

    emit regionBackgroundFileChanged(newBackground);
}

void MapHandler::regionBackgroundTilesetChanged(const QString& newTileset) {
    // TODO Check if newTileset is empty necessary?

    // Ignoring the return value, because it's very unlikely that the image can't be loaded and even if it can't, we don't care
    imageCache->addTileset(gamePaths->getTilesetFile(newTileset), regions[selectedRegion].backgroundImageFile);
    regions[selectedRegion].tilesetBackgroundImageFile = newTileset;

    emit regionBackgroundTilesetFileChanged(newTileset);
}

void MapHandler::regionBackgroundTilesetFileCleared() {
    regions[selectedRegion].tilesetBackgroundImageFile = "";

    emit regionBackgroundTilesetFileChanged("");
}

void MapHandler::selectedRegionChanged(int selectedRegionIndex) {
    selectedRegion = selectedRegionIndex;
}

void MapHandler::profileChanged(const Profile* newProfile) {
    profile = newProfile;
}

void MapHandler::selectedLayerChanged(int newLayer) {
    // This SHOULD be safe, but just in case:
    // TODO Gotta update the enum when more layers are added. Might want to find a different way to do this
    selectedLayer = static_cast<Layer>(newLayer);
}

void MapHandler::setGamePaths(const GamePaths* paths) {
	gamePaths = paths;

    fileHandler.setGamePaths(paths);
}

void MapHandler::setEpisodeHandler(const EpisodeHandler* manager) {
	episodeHandler = manager;
}

void MapHandler::setImageCache(ImageCache* cache) {
    imageCache = cache;
}

const QString& MapHandler::getLastError() const {
    return lastError;
}
