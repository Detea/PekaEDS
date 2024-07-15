#pragma once
#include "AbstractMapFileHandler.h"

#include "PK2LegacyMapData.h"

#include <qstring.h>

/*
	This class reads and writes .map files of the original 1.3 map format.
*/
class LegacyMapFileHandler : public AbstractMapFileHandler {
public:
	virtual void read(const QString& filename, PK2::MapBaseMetadata& metadata, PK2::MapData& mapData) override;

	virtual void write(const QString& filename, const PK2::MapBaseMetadata& metadata, const PK2::MapData& mapData) override;
};
