#pragma once
#include "Weather.h"
#include "Scrolling.h"

#include <string>

/*
	This is the base class for all classes that hold a maps metadata.

	If a new map format with new metadata has been created, inherit from this class.
*/
namespace PK2 {
	struct MapBaseMetadata {
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
	};
}