#pragma once
#include <qstring.h>
#include <qstringview.h>
#include <QtGlobal>

#include "PK2SpriteBase.h"

/*
	This class contains the path to the various content folders of the game as strings.

	Each string that can be retrievied with getXXXPath() ends with the paltform specific file separator character.
*/
class GamePaths {
public:
	GamePaths();

	void setBasePath(const QString& basePath);
    void setCurrentEpisode(const QString& episodeName);

    const QString& tilesetFolder() const;
    const QString& backgroundFolder() const;
    const QString& musicFolder() const;
    const QString& spriteFolder() const;
    const QString& episodesFolder() const;

    const QString& currentEpisodeFolder() const;

	/*
	QString getTilesetFileEpisode(QStringView loadedEpisode, QStringView filename) const;
	QString getBackgroundFileEpisode(QStringView loadedEpisode, QStringView filename) const;
	*/

    QString getSprite(const QString& filename) const;
    QString getSpriteImage(PK2::SpriteBase& sprite) const;

	QString getTilesetFile(const QString& filename) const;
	QString getBackgroundFile(const QString& filename) const;

    QString getMapImage() const;

	// TODO check for loaded episode
	QString getPK2StuffFile() const;

#ifdef Q_OS_LINUX
    // Returns either the found file or the parameter filename if it hasn't been found
    QString getFileCaseInsensitive(const QString& filename, const QString& path) const;
#endif

private:
	QString basePath;

	QString tilesetPath;
	QString backgroundPath;
	
	QString musicPath;
	
	QString spritePath;
	
	QString episodePath;

    QString gfxPath;

    QString currentEpisodeName;
    QString currentEpisodeFolderPath;
};

