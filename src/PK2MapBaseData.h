#pragma once

#include <string>
#include <string_view>
#include <vector>

#include <qimage.h>

#include "PK2SpriteBase.h"

#include "Weather.h"
#include "Scrolling.h"

/*
	This class contains all the metadata that is read from .map files.
	The purpose of this class is to ONLY hold the data, it does not perform any manipulation on it.

	The idea of this struct is that is serves as a base for all (new/extended, too) map formats.
	That means that new map formats should contain all the information that is contained in the 1.3 map format.
*/
namespace PK2 {
	struct MapBaseData {
		std::string name;
		std::string author;
		
		std::string tilesetFile;
		std::string backgroundFile;

		std::string musicFile;

		int levelNumber = 1;
		int time = 0;

		PK2::Weather weatherType = PK2::Weather::NORMAL;
		PK2::Scrolling scrollingType = PK2::Scrolling::STATIC;

		int iconNumber = 0;
		int mapX = 0;
		int mapY = 0;

		int playerSpriteIndex = 0;

		int spritesAmount = 0;
		std::vector<std::string> spriteFiles;
		std::vector<PK2::SpriteBase> spriteObjectList;

		std::vector<std::vector<int>> tileLayers;
		std::vector<std::vector<int>> spriteLayers;
	};
}