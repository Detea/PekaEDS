#include "GamePaths.h"
#include <qdebug.h>
#include <QtGlobal>
#include <QDir>

GamePaths::GamePaths() {

}

void GamePaths::setBasePath(const QString& path) {
	QString separator("/");

	basePath = path;
	basePath += separator;

	tilesetPath = basePath;
	tilesetPath += "gfx";
	tilesetPath += separator;
	tilesetPath += "tiles";
	tilesetPath += separator;

	backgroundPath = basePath;
	backgroundPath += "gfx";
	backgroundPath += separator;
	backgroundPath += "scenery";
	backgroundPath += separator;

	musicPath = basePath;
	musicPath += "music";
	musicPath += separator;

	spritePath = basePath;
	spritePath += "sprites";
	spritePath += separator;

	episodePath = basePath;
	episodePath += "episodes";
	episodePath += separator;

    gfxPath = basePath;
    gfxPath += "gfx";
    gfxPath += separator;
}

#ifdef Q_OS_LINUX
QString GamePaths::getFileCaseInsensitive(const QString& filename, const QString& path) const {
    QStringList files = QDir(path).entryList();
    for (const QString& str : files) {
        if (str.toLower() == filename.toLower()) {
            return path + str;
        }
    }

    return filename;
}
#endif

void GamePaths::setCurrentEpisode(const QString& newCurrentEpisode) {
    currentEpisodeName = newCurrentEpisode;

    currentEpisodeFolderPath = episodePath + currentEpisodeName + "/";
}

const QString& GamePaths::tilesetFolder() const {
	return tilesetPath;
}

const QString& GamePaths::backgroundFolder() const {
	return backgroundPath;
}

const QString& GamePaths::musicFolder() const {
	return musicPath;
}

const QString& GamePaths::spriteFolder() const {
	return spritePath;
}

const QString& GamePaths::episodesFolder() const {
	return episodePath;
}

const QString& GamePaths::currentEpisodeFolder() const {
    return currentEpisodeFolderPath;
}

QString GamePaths::getSprite(const QString& filename) const {
#ifdef Q_OS_LINUX
    if (currentEpisodeName.isEmpty()) {
        return getFileCaseInsensitive(filename, spritePath);
    }

    return getFileCaseInsensitive(filename, currentEpisodeFolderPath);
#else
    if (currentEpisodeName.isEmpty()) {
        return spritePath + filename;
    }

    return currentEpisodeFolderPath + filename;
#endif
}

QString GamePaths::getSpriteImage(PK2::SpriteBase& sprite) const {
#ifdef Q_OS_LINUX
    if (currentEpisodeName.isEmpty()) {
        return getFileCaseInsensitive(sprite.imageFile, spritePath);
    }

    return getFileCaseInsensitive(sprite.imageFile, currentEpisodeFolderPath);
#else
    if (currentEpisodeName.isEmpty()) {
        return spritePath + sprite.imageFile;
    }

    return currentEpisodeFolderPath + sprite.imageFile;
#endif
}

QString GamePaths::getTilesetFile(const QString& filename) const {
#ifdef Q_OS_LINUX
    if (currentEpisodeName.isEmpty()) {
        return getFileCaseInsensitive(filename, tilesetPath);
    }

    return getFileCaseInsensitive(filename, currentEpisodeFolderPath);
#else
    if (currentEpisodeName.isEmpty()) {
        return tilesetPath + filename;
    }

    return currentEpisodeFolderPath + filename;
#endif
}

QString GamePaths::getBackgroundFile(const QString& filename) const {
#ifdef Q_OS_LINUX
    if (currentEpisodeName.isEmpty()) {
        return getFileCaseInsensitive(filename, backgroundPath);
    }

    return getFileCaseInsensitive(filename, currentEpisodeFolderPath);
#elif defined(Q_OS_WIN)
    if (currentEpisodeName.isEmpty()) {
        return backgroundPath + filename;
    }

    return currentEpisodeFolderPath + filename;
#endif
}

QString GamePaths::getMapImage() const {
#ifdef Q_OS_LINUX
    if (currentEpisodeName.isEmpty()) {
        return getFileCaseInsensitive("map.bmp", gfxPath);
    }

    return getFileCaseInsensitive("map.bmp", currentEpisodeFolderPath);
#else
    if (currentEpisodeName.isEmpty()) {
        return gfxPath + "map.bmp";
    }

    return currentEpisodeFolderPath + "map.bmp";
#endif
}

QString GamePaths::getPK2StuffFile() const {
#ifdef Q_OS_LINUX
    if (currentEpisodeName.isEmpty()) {
        return getFileCaseInsensitive("pk2stuff.bmp", gfxPath);
    }

    return getFileCaseInsensitive("pk2stuff.bmp", currentEpisodeFolderPath);
#else
    if (currentEpisodeName.isEmpty()) {
        return basePath + "gfx/pk2stuff.bmp";
    } else {
        return currentEpisodeFolderPath + "/pk2stuff.bmp";
    }
#endif
}
