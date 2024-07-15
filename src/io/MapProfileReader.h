#pragma once
#include <string_view>

#include "MapProfile.h"

class MapProfileReader {
public:
	MapProfileReader();

	void loadProfile(MapProfile* const targetProfile, std::string_view profileFile);
};

