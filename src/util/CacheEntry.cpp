#include "CacheEntry.h"
#include "GFXUtil.h"

#include <QDebug>

CacheEntry::CacheEntry() {}

CacheEntry::CacheEntry(QImage& backgroundImage) {
    colorPalette = backgroundImage.colorTable();

    backgroundPixmap = QPixmap::fromImage(backgroundImage);
}

void CacheEntry::addTileset(const QString tilesetFilename, QImage tilesetImage) {
    GFXUtil::setPalette(colorPalette, tilesetImage);
    GFXUtil::makeTransparent(&tilesetImage);

    pixmaps[tilesetFilename] = QPixmap::fromImage(tilesetImage);
}

void CacheEntry::addSprite(QImage spriteImage, const PK2::SpriteBase& sprite) {
    spriteImage.setColorTable(colorPalette);

    GFXUtil::makeTransparent(&spriteImage);

    pixmaps[sprite.imageFileWithColor] = QPixmap::fromImage(spriteImage);
}

const QPixmap& CacheEntry::getBackground() {
    return backgroundPixmap;
}

const QPixmap& CacheEntry::getTileset(const QString& tilesetFile) {
    // TODO check if it exists?

    return pixmaps[tilesetFile];
}

const QPixmap& CacheEntry::getSprite(const PK2::SpriteBase& sprite) {
    // TODO check if it exists?
    return pixmaps[sprite.imageFileWithColor];
}

bool CacheEntry::hasTileset(const QString& tilesetFile) {
    return pixmaps.count(tilesetFile) != 0;
}

bool CacheEntry::hasSprite(const PK2::SpriteBase& sprite) {
    return pixmaps.contains(sprite.imageFileWithColor);
}

void CacheEntry::setMiniMapColorsFor(const QString& tilesetFilename, QImage& tilesetImage) {
    tilesetMiniMapColors.insert(tilesetFilename, QList<QRgb>{});

    GFXUtil::calculateTileColors(tilesetImage, tilesetMiniMapColors[tilesetFilename]);
}

QList<QRgb>& CacheEntry::getMiniMapColorsFor(const QString& tilesetFile) {
    // TODO What if the tileset hasn't been loaded? Theoretically it should be everytime, but gotta make sure!
    return tilesetMiniMapColors[tilesetFile];
}

const QList<QRgb>& CacheEntry::getPalette() {
    return colorPalette;
}

void CacheEntry::fromCacheEntry(const QImage& backgroundImage, const CacheEntry& otherCacheEntry, const QMap<QString, QImage> tilesetImages, const QMap<QString, QImage> spriteImages) {
    backgroundPixmap = QPixmap::fromImage(backgroundImage);

    colorPalette = backgroundImage.colorTable();

    for (auto [key, value] : tilesetImages.asKeyValueRange()) {
        QImage image = value;

        setMiniMapColorsFor(key, image);
        addTileset(key, image);
    }

    for (auto [key, value] : spriteImages.asKeyValueRange()) {
        QImage spriteImage = value;

        spriteImage.setColorTable(colorPalette);

        GFXUtil::makeTransparent(&spriteImage);

        pixmaps[key] = QPixmap::fromImage(spriteImage);
    }
}
