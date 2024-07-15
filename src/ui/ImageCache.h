#pragma once

#include <qpixmap.h>
#include <qimage.h>
#include <qurl.h>
#include <map>
#include <QMap>

#include "PK2SpriteBase.h"
#include "GamePaths.h"

#include "CacheEntry.h"

/*
	This class stores all the loaded and recolored sprite images.
	Images that have been loaded and processed previously will be reused.

	Use the add() method to load, recolor, make transparent and set a sprites image.
	The processed image will be stored in the sprite.image variable.

	sprite.image will point to one of the cached QPixmaps.

	// TODO Where to handle background image changing? not in here?
*/
class ImageCache {
public:
    ImageCache();

    // TODO How to handle clean up? When to remove an image, when the bg, tileset or sprite gets removed?

	// sprite contains the sprites data, filePath contains the path to the sprite file itself and its image file.
    bool addSprite(PK2::SpriteBase& sprite, const QString& filePath, const QString& backgroundFile);
    void addMissingSprite(PK2::SpriteBase& sprite, const QString& backgroundFile);

    bool hasSpriteImage(PK2::SpriteBase& sprite, const QString& backgroundFile);

    bool loadSpriteImage(const PK2::SpriteBase& sprite, const QString& backgroundFile);
    const QPixmap& getSpriteImage(const PK2::SpriteBase& sprite, const QString& backgroundFile);

    // Returns the recolored sprite image for a region.
    // NOTE: This method expects the sprite's image to be loaded already!
    QPixmap& getRegionSpriteImage(const PK2::SpriteBase& sprite, const QString& backgroundFile);

    // Returns true when the image was loaded or has been loaded before
    bool addBackground(const QString& filepath);

    // oldBackgroundFile needs to be just the background file (castle.bmp), newBackgroundFilePath needs to be the path to the file (/games/Greta/castle.bmp)
    void changeBackground(const QString& oldBackgroundFile, const QString& newBackgroundFilePath);

    // Returns the background or nullptr if it hasn't been loaded
    const QPixmap* getBackground(const QString& filename);
    const QList<QRgb>& getBackgroundPalette(const QString& filename);

    // Returns true when the image was loaded or has been loaded before
    bool addTileset(const QString& tilesetFile, const QString& backgroundFile);
    // Adds a tileset without changing it's color palette
    bool addTileset(const QString& tilesetFile);

    // Important: Only call this method when backgroundFile has definitely been loaded!
    void setTilesetPalette(const QString& tilesetFile, const QString& backgroundFile);

    // Important: Only call this method when you know that the tileset has been loaded!
    const QPixmap& getTileset(const QString& tilesetFilename, const QString& backgroundFilename);

    // Note: tilesetFilePath takes a path to the tileset file, backgroundFilePath takes a path to the background file
    const QPixmap& getOrSetTileset(const QString& tilesetFilePath, const QString& backgroundFilePath);

    QList<QRgb>& getTilesetMiniMapColors(const QString& tilesetFile, const QString& backgroundFile);

    // TODO Remove this!
	// Removes all images from the cache
	void clear();

	// Contains the error string, if one occurred
	const QString& error();

    void setGamePaths(const GamePaths* newGamePaths);

private:
	void updateSpritePalettes();

private:
    QMap<QString, CacheEntry> cacheEntries;
    QMap<QString, QImage> backgroundImages;
    QMap<QString, QImage> tilesetImages;
    QMap<QString, QImage> spriteImages;

	QString errorString;

	QImage missingImage;
	QPixmap missingImagePixmap;

    const GamePaths* gamePaths = nullptr;
};

