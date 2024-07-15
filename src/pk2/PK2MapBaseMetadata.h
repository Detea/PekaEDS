#pragma once
#include "Weather.h"
#include "Scrolling.h"
#include "PK2SpriteBase.h"

#include <qstring.h>

/*
 * The data in this struct belongs to the map and is shared across all regions.
 */
namespace PK2 {
	struct MapBaseMetadata {
		QString name;
		QString author;

		int levelNumber = 1;
		int time = 0;

        int iconNumber = 0;
        int mapX = 0;
        int mapY = 0;

        int playerSpriteIndex = 0;

        int spritesAmount = 0;

        std::vector<QString> spriteFilenames;
        std::vector<PK2::SpriteBase> spriteObjects;

        QString luaScript;
        int gameMode = 0;

		PK2::Weather weatherType = PK2::Weather::NORMAL;
		PK2::Scrolling scrollingType = PK2::Scrolling::STATIC;

        // These are for loading legacy maps only!
        QString foregroundTilesetFile;
        QString backgroundFile;
        QString musicFile;
    };
}
