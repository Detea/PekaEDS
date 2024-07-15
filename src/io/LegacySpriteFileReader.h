#pragma once
#include "AbstractSpriteFileReader.h"

class LegacySpriteFileReader : public AbstractSpriteFileReader {
public:
	void read(const QString& filename, PK2::SpriteBase* sprite) override;

private:
	static constexpr unsigned char ID_1_1[] = { 0x31, 0x2E, 0x31, 0x00 };
	static constexpr unsigned char ID_1_2[] = { 0x31, 0x2E, 0x32, 0x00 };
	static constexpr unsigned char ID_1_3[] = { 0x31, 0x2E, 0x33, 0x00 };
};
