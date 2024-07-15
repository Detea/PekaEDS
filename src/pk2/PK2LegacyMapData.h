#pragma once

#include <vector>

#include <qimage.h>

#include "PK2SpriteBase.h"

#include "Weather.h"
#include "Scrolling.h"

namespace PK2 {
    struct LegacyMapData {
        QString name;
        QString author;
		
        QString tilesetFile;
        QString backgroundFile;

        QString musicFile;

		int levelNumber = 1;
		int time = 0;

		PK2::Weather weatherType = PK2::Weather::NORMAL;
		PK2::Scrolling scrollingType = PK2::Scrolling::STATIC;

		int iconNumber = 0;
		int mapX = 0;
		int mapY = 0;

		int playerSpriteIndex = 0;

		int spritesAmount = 0;
        std::vector<QString> spriteFilenames;
		std::vector<PK2::SpriteBase> spriteObjectList;

        std::vector<int> backgroundLayer;
        std::vector<int> foregroundLayer;
        std::vector<int> spriteLayer;
	};
}
