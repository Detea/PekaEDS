#pragma once

#include "PK2MapBaseMetadata.h"
#include "PK2MapData.h"

#include <string_view>

class AbstractMapFileHandler {
public:
	virtual void read(const QString& filename, PK2::MapBaseMetadata& metadata, PK2::MapData& mapData) = 0;

	virtual void write(const QString& filename, const PK2::MapBaseMetadata& metadata, const PK2::MapData& mapData) = 0;
};