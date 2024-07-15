#include "LegacyMapFileHandler.h"
#include "PK2LegacyMapData.h"

#include "PK2FileUtil.h"
#include "LegacyMapConstants.h"

#include "ExpectedMapFormatException.h"

#include "PK2FileUtil.h"

#include <qdebug.h>

#include <string>

#include <fstream>

void LegacyMapFileHandler::read(const QString& filename, PK2::MapBaseMetadata& metadata, PK2::MapData& mapData) {
	std::ifstream in;

	in.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try {
		in.open(filename.toStdString().c_str(), std::ios::in | std::ios::binary);
		 
		unsigned char versionStr[5];

		in.read(reinterpret_cast<char*>(&versionStr), sizeof(versionStr));

		if (!PK2FileUtil::versionMatch(versionStr, Map::LegacyMapConstants::ID)) {
			throw WrongMapFormatException("1.3");
		} else {
			using namespace Map;

			metadata.foregroundTilesetFile = PK2FileUtil::readString(in, LegacyMapConstants::FILE_CHARACTER_LIMIT);
			metadata.backgroundFile = PK2FileUtil::readString(in, LegacyMapConstants::FILE_CHARACTER_LIMIT);
			metadata.musicFile = PK2FileUtil::readString(in, LegacyMapConstants::FILE_CHARACTER_LIMIT);
		
			metadata.name = PK2FileUtil::readString(in, LegacyMapConstants::AUTHOR_MAP_NAME_LENGTH);
			metadata.author = PK2FileUtil::readString(in, LegacyMapConstants::AUTHOR_MAP_NAME_LENGTH);

			metadata.levelNumber = PK2FileUtil::readPK2Int(in);
			metadata.weatherType = static_cast<PK2::Weather>(PK2FileUtil::readPK2Int(in));
			
			// switch values
			PK2FileUtil::readPK2Int(in);
			PK2FileUtil::readPK2Int(in);
			PK2FileUtil::readPK2Int(in);

			metadata.time = PK2FileUtil::readPK2Int(in);

			// "Extra" - not used.
			PK2FileUtil::readPK2Int(in);

			metadata.scrollingType = static_cast<PK2::Scrolling>(PK2FileUtil::readPK2Int(in));

			metadata.playerSpriteIndex = PK2FileUtil::readPK2Int(in);

			metadata.mapX = PK2FileUtil::readPK2Int(in);
			metadata.mapY = PK2FileUtil::readPK2Int(in);
		
			metadata.iconNumber = PK2FileUtil::readPK2Int(in);

			metadata.spritesAmount = PK2FileUtil::readPK2Int(in);

			mapData.spriteFiles.clear();
			mapData.spriteFiles.reserve(metadata.spritesAmount);
			for (int i = 0; i < metadata.spritesAmount; ++i) {
				mapData.spriteFiles.push_back(PK2FileUtil::readString(in, LegacyMapConstants::FILE_CHARACTER_LIMIT));
			}

			mapData.tileLayers.clear();
			mapData.tileLayers.push_back(std::vector<int>(LegacyMapConstants::MAP_SIZE, 255)); // background layer
			mapData.tileLayers.push_back(std::vector<int>(LegacyMapConstants::MAP_SIZE, 255)); // foreground layer

            PK2FileUtil::readLayer(in, mapData.tileLayers[0], LegacyMapConstants::MAP_WIDTH, LegacyMapConstants::MAP_HEIGHT, true);
            PK2FileUtil::readLayer(in, mapData.tileLayers[1], LegacyMapConstants::MAP_WIDTH, LegacyMapConstants::MAP_HEIGHT, true);

			mapData.spriteLayers.clear();
			mapData.spriteLayers.push_back(std::vector<int>(LegacyMapConstants::MAP_SIZE, 255));
            PK2FileUtil::readLayer(in, mapData.spriteLayers[0], LegacyMapConstants::MAP_WIDTH, LegacyMapConstants::MAP_HEIGHT, true);
		}
	} catch (std::ifstream::failure& e) {
		throw;
	}
}

void LegacyMapFileHandler::write(const QString& filename, const PK2::MapBaseMetadata& metadata, const PK2::MapData& mapData) {
	// TODO
}
