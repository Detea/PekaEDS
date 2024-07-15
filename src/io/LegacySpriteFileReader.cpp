#include "LegacySpriteFileReader.h"
#include "WrongSpriteFormatException.h"

#include <fstream>
#include "PK2FileUtil.h"

#include <qdebug.h>
#include <qfileinfo.h>

void LegacySpriteFileReader::read(const QString& filename, PK2::SpriteBase* sprite) {
	std::ifstream in;

	in.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try {
        std::ifstream in;

        in.open(filename.toStdString().c_str(), std::ios::in | std::ios::binary);

		unsigned char version[4];
	
		in.read(reinterpret_cast<char*>(&version), sizeof(version));

		char sprVersion = 0;

		if (std::equal(std::begin(version), std::end(version), std::begin(ID_1_1))) sprVersion = 11;
		if (std::equal(std::begin(version), std::end(version), std::begin(ID_1_2))) sprVersion = 12;
		if (std::equal(std::begin(version), std::end(version), std::begin(ID_1_3))) sprVersion = 13;

		if (sprVersion != 11 && sprVersion != 12 && sprVersion != 13) {
			// All custom and most official sprites are at least version 1.3, so we expect that one for legacy sprites
			throw WrongSpriteFormatException("1.3", filename);
		}

		sprite->filename = QFileInfo(filename).fileName();

		int discardedInt = 0;
		unsigned char discardedByte;
		bool discardedBool;

		// Used - Sprite type
		in.read(reinterpret_cast<char*>(&sprite->type), sizeof(sprite->type));

		// Used - Sprites image sheet
		if (sprVersion == 11 || sprVersion == 12) {
			sprite->imageFile = PK2FileUtil::readString(in, 13);
		} else if (sprVersion == 13) {
			sprite->imageFile = PK2FileUtil::readString(in, 100);
		}

		// Not used - Sound files
		char discardedString11_12[13];
		char discardedString13[100];
		for (int i = 0; i < 7; ++i) {
			if (sprVersion == 11 || sprVersion == 12) {
				in.read(reinterpret_cast<char*>(&discardedString11_12), sizeof(discardedString11_12));
			} else {
				in.read(reinterpret_cast<char*>(&discardedString13), sizeof(discardedString13));
			}
		}

		// Not used - Sounds
		for (int i = 0; i < 7; ++i) {
			in.read(reinterpret_cast<char*>(&discardedInt), sizeof(discardedInt));
		}

		// Not used - Frame amount
		in.read(reinterpret_cast<char*>(&discardedByte), sizeof(discardedByte));

		// Not used - Animation data
		for (int i = 0; i < 20; ++i) {
			for (int j = 0; j < 10; ++j) {
				in.read(reinterpret_cast<char*>(&discardedByte), sizeof(discardedByte));
			}

			in.read(reinterpret_cast<char*>(&discardedByte), sizeof(discardedByte));
			in.read(reinterpret_cast<char*>(&discardedBool), sizeof(discardedBool));
		}

		// Not used - Animation amount
		in.read(reinterpret_cast<char*>(&discardedByte), sizeof(discardedByte));

		// Not used - Animation Frame Rate
		in.read(reinterpret_cast<char*>(&discardedByte), sizeof(discardedByte));

		// Not used - Unknown
		in.read(reinterpret_cast<char*>(&discardedByte), sizeof(discardedByte));

		// Used
		in.read(reinterpret_cast<char*>(&sprite->frameX), sizeof(sprite->frameX));
		in.read(reinterpret_cast<char*>(&sprite->frameY), sizeof(sprite->frameY));

		// Used
		in.read(reinterpret_cast<char*>(&sprite->frameWidth), sizeof(sprite->frameWidth));
		in.read(reinterpret_cast<char*>(&sprite->frameHeight), sizeof(sprite->frameHeight));

		// Unused - Distance between frames
		in.read(reinterpret_cast<char*>(&discardedInt), sizeof(discardedInt));

		// Used
		int nameLength = 32;
		if (sprVersion == 11 || sprVersion == 12) {
			nameLength = 30;
		}

		sprite->name = PK2FileUtil::readString(in, nameLength);

		// Unused - width
		in.read(reinterpret_cast<char*>(&discardedInt), sizeof(discardedInt));

		// Unused - height
		in.read(reinterpret_cast<char*>(&discardedInt), sizeof(discardedInt));

		double discardedDouble;
		// Unused - weight
		in.read(reinterpret_cast<char*>(&discardedDouble), sizeof(discardedDouble));

		// Unused - Is enemy?
		in.read(reinterpret_cast<char*>(&discardedBool), sizeof(discardedBool));

		// Unused
		in.read(reinterpret_cast<char*>(&discardedByte), sizeof(discardedByte));
		in.read(reinterpret_cast<char*>(&discardedByte), sizeof(discardedByte));
		in.read(reinterpret_cast<char*>(&discardedByte), sizeof(discardedByte));

		// Unused - energy
		in.read(reinterpret_cast<char*>(&discardedInt), sizeof(discardedInt));

		// Unused - damage
		in.read(reinterpret_cast<char*>(&discardedInt), sizeof(discardedInt));

		// Unused - Damage type
		in.read(reinterpret_cast<char*>(&discardedByte), sizeof(discardedByte));

		// Unused - Immunity
		in.read(reinterpret_cast<char*>(&discardedByte), sizeof(discardedByte));

		// Unused
		in.read(reinterpret_cast<char*>(&discardedByte), sizeof(discardedByte));
		in.read(reinterpret_cast<char*>(&discardedByte), sizeof(discardedByte));

		// Unused - Score
		in.read(reinterpret_cast<char*>(&discardedInt), sizeof(discardedInt));

		// Unused - AI
		int aiCount = 10;
		
		if (sprVersion == 11 || sprVersion == 12) {
			aiCount = 5;
		}

		for (int i = 0; i < aiCount; ++i) {
			in.read(reinterpret_cast<char*>(&discardedInt), sizeof(discardedInt));
		}

		// Unused - Max Jump
		in.read(reinterpret_cast<char*>(&discardedByte), sizeof(discardedByte));

		// Unused
		in.read(reinterpret_cast<char*>(&discardedByte), sizeof(discardedByte));
		in.read(reinterpret_cast<char*>(&discardedByte), sizeof(discardedByte));
		in.read(reinterpret_cast<char*>(&discardedByte), sizeof(discardedByte));

		// Unused - Max Speed
		in.read(reinterpret_cast<char*>(&discardedDouble), sizeof(discardedDouble));

		// Unused - Load time
		in.read(reinterpret_cast<char*>(&discardedInt), sizeof(discardedInt));

		// Used - color
		in.read(reinterpret_cast<char*>(&sprite->color), sizeof(sprite->color));
	} catch (const std::ifstream::failure& e) {
		throw;
	}
}
