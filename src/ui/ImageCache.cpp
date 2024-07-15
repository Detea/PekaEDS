#include "ImageCache.h"
#include "GFXUtil.h"

#include <qdebug.h>

#define SET_SPRITE_MISSING(sprite)	\
    sprite.frameX = 0;	\
    sprite.frameY = 0;	\
    sprite.frameWidth = 32;	\
    sprite.frameHeight = 32; \
    sprite.imageFile = "missing";\
    sprite.color = 255;\

ImageCache::ImageCache() {
	// TODO Do I need to store the QImage or can I just convert it directly?
	// How do missing sprites act when the palette changes?
	missingImage = QImage(":/res/missing.bmp");

	missingImagePixmap = QPixmap::fromImage(missingImage.copy(0, 0, 32, 32));
}

bool ImageCache::addSprite(PK2::SpriteBase& sprite, const QString& filePath, const QString& backgroundFile) {
    QImage* image = nullptr;
    QImage loadedImage;
    bool imageLoaded = false;

    if (!spriteImages.contains(sprite.imageFileWithColor)) {
        if (loadedImage.load(filePath)) {
            image = &loadedImage;

            if (sprite.color != 255) GFXUtil::recolorSprite(loadedImage, sprite.color);
            spriteImages[sprite.imageFileWithColor] = loadedImage;

            imageLoaded = true;
        } else {
            image = &missingImage;

            SET_SPRITE_MISSING(sprite)
        }
    } else {
        image = &spriteImages[sprite.imageFileWithColor];

        imageLoaded = true;
    }

    cacheEntries[backgroundFile].addSprite(*image, sprite);

    return imageLoaded;
}

void ImageCache::addMissingSprite(PK2::SpriteBase& sprite, const QString& backgroundFile) {
    SET_SPRITE_MISSING(sprite)
    cacheEntries[backgroundFile].addSprite(missingImage, sprite);
}

bool ImageCache::hasSpriteImage(PK2::SpriteBase& sprite, const QString& backgroundFile) {
    return cacheEntries[backgroundFile].hasSprite(sprite);
}

const QPixmap& ImageCache::getSpriteImage(const PK2::SpriteBase& sprite, const QString& backgroundFile) {
    // TODO IMPORTANT What if the file hasn't been loaded?

    return cacheEntries[backgroundFile].getSprite(sprite);
}

bool ImageCache::addBackground(const QString& filepath) {
    QStringList split = filepath.split("/");
    QString filename = split.last().toLower();

    if (cacheEntries.contains(filename)) return true;

    if (!backgroundImages.contains(filename)) {
        if (QImage image; image.load(filepath)) {
            cacheEntries[filename] = CacheEntry(image);
        } else {
            cacheEntries[filename] = CacheEntry(missingImage);

            errorString = "Unable to find background file. File does not exist.";

            return false;
        }
    } else {
        cacheEntries[filename] = CacheEntry(backgroundImages[filename]);
    }

    return true;
}

void ImageCache::changeBackground(const QString& oldBackgroundFile, const QString& newBackgroundFilePath) {
    QStringList split = newBackgroundFilePath.split("/");
    QString filename = split.last().toLower();

    QImage image;

    if (!backgroundImages.contains(filename)) {
        // This SHOULD never happen, because the user just selected newBackgroundFilePath from the FileDialog, but you never know...
        if (image.load(newBackgroundFilePath)) {
            backgroundImages[filename] = image;
        } else {
            image = missingImage;

            errorString = "Unable to find background file. File does not exist.";
        }
    }

    // oldBackgroundFile should definitely be in there, but just to be safe...
    Q_ASSERT(cacheEntries.contains(oldBackgroundFile));

    CacheEntry ce;
    ce.fromCacheEntry(backgroundImages[filename], cacheEntries[oldBackgroundFile], tilesetImages, spriteImages);
    cacheEntries[filename] = ce;
}

const QPixmap* ImageCache::getBackground(const QString& filename) {
    if (cacheEntries.contains(filename)) {
        return &cacheEntries[filename].getBackground();
    }

    return nullptr;
}

const QList<QRgb>& ImageCache::getBackgroundPalette(const QString& filename) {
    return cacheEntries[filename].getPalette();
}

bool ImageCache::addTileset(const QString& tilesetFilePath, const QString& backgroundFilename) {
    QStringList split = tilesetFilePath.split("/"); // TODO Does this work on windows?
    QString tilesetFile = split.last();

    if (cacheEntries[backgroundFilename].hasTileset(tilesetFilePath)) return true;

    QImage* finalImage = nullptr;
    QImage loadedImage;

    if (tilesetImages.contains(tilesetFile)) {
        finalImage = &tilesetImages[tilesetFile];
    } else {
        if (loadedImage.load(tilesetFilePath)) {
            finalImage = &loadedImage;

            tilesetImages[tilesetFile] = loadedImage;
        } else {
            finalImage = &missingImage;

            errorString = "Unable to find tileset file.";
        }
    }

    // Set the color while the image is still in the 8 bit indexed format
    cacheEntries[backgroundFilename].setMiniMapColorsFor(tilesetFile, *finalImage);

    // TODO Check if it exists?
    cacheEntries[backgroundFilename].addTileset(tilesetFile, *finalImage);

    return true;
}

const QPixmap& ImageCache::getTileset(const QString& tilesetFilename, const QString& backgroundFilename) {
    // TODO What if the tileset hasn't been loaded?

    QStringList split = tilesetFilename.split("/"); // TODO Does this work on windows?
    QString tilesetFile = split.last();

    return cacheEntries[backgroundFilename].getTileset(tilesetFile);
}

QList<QRgb>& ImageCache::getTilesetMiniMapColors(const QString& tilesetFile, const QString& backgroundFile) {
    // TODO Should check if backgroundFile is loaded?

    return cacheEntries[backgroundFile].getMiniMapColorsFor(tilesetFile);
}

void ImageCache::updateSpritePalettes() {
	// TODO
}

const QString& ImageCache::error() {
	return errorString;
}

void ImageCache::setGamePaths(const GamePaths* newGamePaths) {
    gamePaths = newGamePaths;
}
