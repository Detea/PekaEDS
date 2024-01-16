#pragma once
#include "PK2SpriteBase.h"

#include <vector>

/*
	This class holds only the following data:
		- Tile layers
		- Sprite layers
		- List of sprite objects

	This is a data only class, to add/remove tiles and/or sprites use EDS::PK2Map::setTile, getTile, addSprite, etc.
*/
namespace PK2 {
	struct MapData {
		std::vector<std::string> spriteFiles;
		std::vector<PK2::SpriteBase> spriteObjectList;

		std::vector<std::vector<int>> tileLayers;
		std::vector<std::vector<int>> spriteLayers;
	};
}