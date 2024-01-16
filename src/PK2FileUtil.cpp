#include "PK2FileUtil.h"

#include <qdebug.h>
#include <string>

bool PK2FileUtil::versionMatch(const unsigned char expect[5], const unsigned char provided[5]) {
	for (int i = 0; i < 5; i++) {
		if (expect[i] != provided[i]) {
			return false;
		}
	}

	return true;
}


bool PK2FileUtil::isPK2StringTerminator(char c) {
	return c == (char)0x0 || c == (char)0xCD || c == (char)0xCC;
}

std::string PK2FileUtil::readString(std::ifstream& in, int size) {
	std::string str = "";

	bool terminaterFound = false;
	for (int i = 0; i < size; i++) {
		char c = 0;

		in.read(reinterpret_cast<char*>(&c), sizeof(c));

		if (!terminaterFound) {
			if (!isPK2StringTerminator(c)) {
				str += c;
			} else {
				terminaterFound = true;
			}
		}
	}

	return str;
}

int PK2FileUtil::readPK2Int(std::ifstream& in) {
	int val = -1;

	std::string str = "";

	for (int i = 0; i < 8; i++) {
		char c = 0;

		in.read(reinterpret_cast<char*>(&c), sizeof(c));

		str += c;
	}

	val = std::stoi(str);

	return val;
}

// TODO Rewrite this, assume that the vector is empty
void PK2FileUtil::readLayer(std::ifstream& in, std::vector<int>& layer, int mapWidth, int mapHeight) {
	int startX = readPK2Int(in);
	int startY = readPK2Int(in);

	int width = readPK2Int(in);
	int height = readPK2Int(in);

	for (int y = startY; y <= startY + height; y++) {
		for (int x = startX; x <= startX + width; x++) {
			unsigned char tile = 255;

			in.read(reinterpret_cast<char*>(&tile), sizeof(tile));

			layer[x + mapWidth * y] = tile;
		}
	}
}