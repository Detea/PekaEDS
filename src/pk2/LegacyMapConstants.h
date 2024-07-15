#pragma once
#include <string_view>

namespace Map {
	struct LegacyMapConstants {
		static constexpr unsigned char ID[5] = { 0x31, 0x2E, 0x33, 0x00, 0xCD };

		static constexpr int FILE_CHARACTER_LIMIT = 13;
		static constexpr int AUTHOR_MAP_NAME_LENGTH = 40;

		static constexpr int MAX_SPRITE_FILES = 100;

		static constexpr int MAP_WIDTH = 256;
		static constexpr int MAP_HEIGHT = 224;
		static constexpr int MAP_SIZE = MAP_WIDTH * MAP_HEIGHT;

		static constexpr int MUSIC_FORMAT_AMOUNT = 4;
		static constexpr std::string_view MUSIC_FORMATS[MUSIC_FORMAT_AMOUNT] = {
			"xm", "mod", "it", "s3m"
		};
	};
}