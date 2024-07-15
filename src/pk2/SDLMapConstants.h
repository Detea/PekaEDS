#pragma once
#include <string_view>

namespace Map {
	struct SDLMapConstants {
		static constexpr int FILE_CHARACTER_LIMIT = 13;
		static constexpr int AUTHOR_MAP_NAME_LENGTH = 40;

		static constexpr int MAX_SPRITE_FILES = 100;

		static constexpr int MUSIC_FORMAT_AMOUNT = 6;
		static constexpr std::string_view MUSIC_FORMATS[MUSIC_FORMAT_AMOUNT] = {
			"xm", "mod", "it", "s3m", "ogg", "mp3"
		};
	};
}