#pragma once
#include "AbstractMapFileHandler.h"

/*
	This class reads and writes .map files of the original 1.3 map format.
*/
class LegacyMapFileHandler : public AbstractMapFileHandler {
public:
	virtual void read(std::string_view filename, PK2::MapBaseMetadata& metadata, PK2::MapData& mapData) override;

	virtual void write(std::string_view filename, const PK2::MapBaseMetadata& metadata, const PK2::MapData& mapData) override;
};