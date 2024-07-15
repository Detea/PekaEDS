#pragma once
#include "PK2SpriteBase.h"

#include "PK2MapBaseMetadata.h"
#include "PK2MapData.h"

#include "GamePaths.h"
#include "EpisodeHandler.h"

#include "ImageCache.h"

#include "Region.h"

#include "FileHandler.h"

#include "Profile.h"

#include "Layers.h"

#include <qobject.h>
#include <qimage.h>
#include <qpixmap.h>
#include <qstring.h>
#include <qfile.h>

#include <vector>

/*
	This map contains an instance of the MapDataBase class, the tileset and background images and the sprite list.

	This classes purpose is to process the map data and to load/hold the data in the way the editor needs it.
    Any changes to the map will be made through this class via slots.
*/
class MapHandler : public QObject {
	Q_OBJECT

public:
    enum class Result {
        SUCCESS, WRONG_MAP_FORMAT, WRONG_SPRITE_FORMAT, UNABLE_TO_READ_FILE, FAILURE
    };

	MapHandler();

	/*
        This methods loads the map file in filename using the correct FileHandler.
	*/
    Result loadFile(const QString& filename);

	void setGamePaths(const GamePaths* paths);
	void setEpisodeHandler(const EpisodeHandler* handler);

    void setImageCache(ImageCache* cache);

    const Region& getRegion(int regionNumber);

    const QString& getLastError() const;

public slots:
    void spritesAdded(const QStringList& filePaths);
    void spriteReplaced(int originalSpriteIndex, const QString& newSpriteFilePath);
    void spriteRemoved(int spriteListIndex);
    void spriteSetAsPlayer(int newPlayerIndex);

    void regionAdded(const Region& addedRegion);
    void regionRemoved(int regionIndex);

    void profileChanged(const Profile* profile);

    void tilePlaced(int x, int y, int selectionWidth, int selectionHeight, const std::vector<int>& selectedTiles);
    void spritePlaced(int x, int y, int selectionWidth, int selectionHeight, const std::vector<int>& selectedSprites);

    void grabTiles(const QRect& selectionRect);
    void grabSprites(const QRect& selectionRect);

    void regionTilesetChanged(const QString& newTileset);
    void regionBackgroundChanged(const QString& newBackground);
    void regionBackgroundTilesetChanged(const QString& newTileset);
    void regionBackgroundTilesetFileCleared();

    void selectedLayerChanged(int newLayer);
    void selectedRegionChanged(int selectedRegionIndex);

    void saveMapMetadata(PK2::MapBaseMetadata& metadata);
    void saveRegions(std::vector<Region>& regionList);

signals:
    void regionForegroundTilesetFileChanged(const QString& newTileset);
    void regionBackgroundFileChanged(const QString& newBackground);
    void regionBackgroundTilesetFileChanged(const QString& newTileset);

    void errorsEncountered(const std::vector<QString>& errorList);
	void spriteListLoaded(std::vector<QString>* spriteFilenames, std::vector<PK2::SpriteBase>* spriteList);

	void mapMetadataChanged(const PK2::MapBaseMetadata* metadata);

	// Catch this signal only when you want to read from the map data. To manipulate it use one of the methods above.
	void mapDataChanged(const PK2::MapData& mapData);

	// Emit these for the save reminder
	// Emit this when the tile or sprite layer gets changed
	// Or when a sprite has been added, removed or replaced
    void saveReminderTriggered();

    void mapChanged();

    void mapRegionsLoaded(const std::vector<Region>* regionList);

    void updateMap(int x, int y, int selectionWidth, int selectionHeight, Layer layer);

    void regionListUpdated();

private:
	void createMissingImagePixmap();

    void loadSpriteList(std::vector<QString>* errorList);

    void convertV13ToV15(const PK2::LegacyMapData& legacyMapData);

private:
    static constexpr char MAP_HEADER_13[4] = { '1', '.', '3', 0x0};
    static constexpr char MAP_HEADER_15[4] = { '1', '.', '5', 0x0};

	int mapWidth = 256;
	int mapHeight = 224;

	// TODO When can missingImage be converted to a pixmap?
	QImage missingImage;
	QPixmap missingImagePixmap;

    // Contains all the data for the map that is shared between all regions
	PK2::MapBaseMetadata metadata;

    int selectedRegion = 0;
    std::vector<Region> regions;

	const GamePaths* gamePaths = nullptr;
	const EpisodeHandler* episodeHandler = nullptr;

    ImageCache* imageCache = nullptr;

    const Profile* profile = nullptr;

    Layer selectedLayer = Layer::BOTH;

    FileHandler fileHandler;

    QString lastError;
};
