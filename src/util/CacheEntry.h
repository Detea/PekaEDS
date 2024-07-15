#ifndef CACHEENTRY_H
#define CACHEENTRY_H

#include <QString>
#include <QImage>
#include <QMap>

#include "PK2SpriteBase.h"

class CacheEntry {
public:
    CacheEntry();
    CacheEntry(QImage& backgroundImage);

    // Note: These two methods do not check if the images have already been loaded!
    // Note: Taking a copy instead of a reference is intentional here!
    void addTileset(const QString tilesetFilename, QImage tilesetImage);

    // Note: This method does not recolor the image (according to the sprite.color value), this is done in ImageCache.
    // Note: This method intentionally creates a copy of the image to not change the original, because it makes the 255th color transparent.
    void addSprite(QImage spriteImage, const PK2::SpriteBase& sprite);

    const QPixmap& getBackground();
    const QPixmap& getTileset(const QString& tilesetFile);
    const QPixmap& getSprite(const PK2::SpriteBase& sprite);

    bool hasTileset(const QString& tilesetFile);
    bool hasSprite(const PK2::SpriteBase& sprite);

    void setMiniMapColorsFor(const QString& tilesetFile, QImage& image);
    QList<QRgb>& getMiniMapColorsFor(const QString& tilesetFile);

    const QList<QRgb>& getPalette();

    /*
     * Not using a copy constructor here because this is a special case.
     * This isn't a copy, this is for when creating a new CacheEntry that should have the same data but a different background.
     */
    void fromCacheEntry(const QImage& backgroundImage, const CacheEntry& otherCacheEntry, const QMap<QString, QImage> tilesetImages, const QMap<QString, QImage> spriteImages);

private:
    QList<QRgb> colorPalette;
    QPixmap backgroundPixmap;   // Used for the actual drawing of the background

    QMap<QString, QPixmap> pixmaps;
    QMap<QString, QList<QRgb>> tilesetMiniMapColors;
};

#endif // CACHEENTRY_H
