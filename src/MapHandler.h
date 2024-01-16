#pragma once
#include "PK2MapBaseData.h"
#include "PK2SpriteBase.h"

#include "PK2MapBaseMetadata.h"
#include "PK2MapData.h"

#include "GamePaths.h"
#include "EpisodeHandler.h"

#include "LegacyMapFileHandler.h"
#include "LegacySpriteFileReader.h"

#include <qobject.h>
#include <qimage.h>
#include <qpixmap.h>
#include <qstring.h>

#include <vector>
#include <map>

/*
	This map contains an instance of the MapDataBase class, the tileset and background images and the sprite list.

	This classes purpose is to process the map data and to load/hold the data in the way the editor needs it.
*/
class MapHandler : public QObject {
	Q_OBJECT

public:
	MapHandler();

	/*
		This methods loads the map file in filename using the correct MapFileHandler.

		Might throw the following exceptions:
		std::ios::failure, ExpectedMapFormatException
	*/
	void loadFile(std::string_view filename);

	// NOTE: This method has to be called before setTileset(), loadTileset() and loadSpriteList() are called, because they all depent on the background image's color palette.
	bool loadBackground(std::string_view filename);

	bool loadTileset(std::string_view filename);

	std::string_view getBackground();
	std::string_view getTileset();

	void setGamePaths(const GamePaths* paths);
	void setEpisodeHandler(const EpisodeHandler* handler);

	void setTileAt(int x, int y, int tile, int layer);

	/*
	void addSprite(std::string_view filename);
	void removeSprite(std::string_view filename);
	void replaceSprite(std::string_view oldSpriteFileName, std::string_view newSpriteFileName);

	void setSpriteAt(int x, int y, int spriteIndex);
	*/

signals:
	// This signal gets emitted after the tileset image has been loaded and its palette has been so to the one the background uses.
	// The 256th color has not been made transparent at this point.
	void tilesetImageLoaded(const QImage* image);

	void tilesetChanged(const QImage* newTileset);
	void backgroundChanged(const QImage* newBackground);

	void missingTileset(std::string_view filename);
	void missingBackground(std::string_view filename);

	void spriteLoadingErrors(const std::vector<QString>* errorList);

	void mapMetadataChanged(const PK2::MapBaseMetadata* metadata);

	// Catch this signal only when you want to read from the map data. To manipulate it use one of the methods above.
	void mapDataChanged(const PK2::MapData& mapData);

	// Emit these for the save reminder
	// Emit this when the tile or sprite layer gets changed
	// Or when a sprite has been added, removed or replaced
	void mapEdited();

private:
	void createMissingImagePixmap();

	void adjustPaletteAndColors();

	void loadSpriteList(std::vector<QString>* errorList);

private:
	int mapWidth = 256;
	int mapHeight = 224;

	// TODO When can missingImage be converted to a pixmap?
	QImage missingImage;
	QPixmap missingImagePixmap;

	PK2::MapBaseMetadata metadata;
	PK2::MapData mapData;

	LegacyMapFileHandler legacyMapFileHandler;
	LegacySpriteFileReader legacySpriteReader;

	QImage tilesetImage;
	QImage backgroundImage;

	std::map<std::string, QPixmap> spriteImages;
	std::vector<PK2::SpriteBase> spriteList;

	const GamePaths* gamePaths = nullptr;
	const EpisodeHandler* episodeHandler = nullptr;
};