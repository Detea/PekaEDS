#pragma once

#include <vector>
#include <fstream>
#include <qstring.h>

#include <nlohmann/json.hpp>

class PK2FileUtil {
public:
    enum Compression {
        NONE = 0, NEW = 1, LEGACY = 2
    };

	static bool versionMatch(const unsigned char expected[5], const unsigned char provided[5]);
	static bool isPK2StringTerminator(char c);

	static QString readString(std::ifstream& in, int size);

    static int readPK2Int(std::ifstream& in);
    static void writePK2Int(std::ofstream& out, int value);

    static void readLayer(std::ifstream& in, std::vector<int>& layer, int mapWidth, int mapHeight, int compression = Compression::LEGACY);
    static void writeLayer(std::ofstream& out, const std::vector<int>& layer, int width, int height, int compression);

    static void writeJsonAsCbor(std::ofstream& out, const nlohmann::json& json);
};
