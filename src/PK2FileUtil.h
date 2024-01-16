#pragma once

#include <vector>
#include <fstream>

class PK2FileUtil {
public:
	static bool versionMatch(const unsigned char expected[5], const unsigned char provided[5]);
	static bool isPK2StringTerminator(char c);

	static std::string readString(std::ifstream& in, int size);

	static int readPK2Int(std::ifstream& in);

	static void readLayer(std::ifstream& in, std::vector<int>& layer, int mapWidth = 256, int mapHeight = 224);
};