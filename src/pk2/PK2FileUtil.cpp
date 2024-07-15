#include "PK2FileUtil.h"

#include <qdebug.h>

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

QString PK2FileUtil::readString(std::ifstream& in, int size) {
	QString str = "";

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

    char data[8];
    in.read(data, sizeof(char) * 8);

    val = std::atoi(data);

	return val;
}

void PK2FileUtil::writePK2Int(std::ofstream& out, int value) {
    std::string strVal = std::to_string(value);
    if (strVal.length() > 7) {
        strVal = strVal.substr(0, 7);
    }

    int length = 8;
    char data[length];
    for (int i = 0; i < length; ++i) {
        data[i] = 0;
    }

    // Leave room for the null terminator
    int charAmount = strVal.length() > 7 ? 7 : strVal.length();
    for (int i = 0; i < charAmount; ++i) {
        data[i] = strVal[i];
    }

    out.write(data, sizeof(char) * length);
}

void PK2FileUtil::readLayer(std::ifstream& in, std::vector<int>& layer, int mapWidth, int mapHeight, int compression) {
    if (compression == Compression::LEGACY) {
        int startX = readPK2Int(in);
        int startY = readPK2Int(in);

        int endX = readPK2Int(in);
        int endY = readPK2Int(in);

        for (int y = startY; y <= startY + endY; y++) {
            for (int x = startX; x <= startX + endX; x++) {
                unsigned char tile = 255;

                in.read(reinterpret_cast<char*>(&tile), sizeof(tile));

                layer[x + y * mapWidth] = tile; // TODO [CHECK] is x + width * y correct or should it be x + y * width?
            }
        }
    } else if (compression == Compression::NONE) {
        for (int i = 0; i < mapWidth * mapHeight; ++i) {
            unsigned char tile = 255;

            in.read(reinterpret_cast<char*>(&tile), sizeof(tile));

            layer[i] = tile;
        }
    } else {
        qDebug() << "Unknown compression!";
    }
}

void PK2FileUtil::writeLayer(std::ofstream& out, const std::vector<int>& layer, int layerWidth, int layerHeight, int compression) {
    int startX = layerWidth;
    int startY = layerHeight;
    int endX = 0;
    int endY = 0;

    int width = 0;
    int height = 0;

    // TODO [FIX] Writing with legacy compression is broken
    // Calculate the area where tiles have been placed within to avoid having to write an unnecessary amount of empty tiles
    if (compression == Compression::LEGACY) {
        for (int y = 0; y < layerHeight; ++y) {
            for (int x = 0; x < layerWidth; ++x) {
                if (layer[x + y * layerWidth] != 255) { // TODO [CHECK] Is x + width * y correct? or is it width * x + y?
                    if (x < startX) {
                        startX = x;
                    }

                    if (y < startY) {
                        startY = y;
                    }

                    if (x > endX) {
                        endX = x;
                    }

                    if (y > endY) {
                        endY = y;
                    }
                }
            }
        }

        if (endX < startX || endY < startY) {
            startX = 0;
            startY = 0;

            endX = 1;
            endY = 1;
        }

        width = endX - startX;
        height = endY - startY;

        writePK2Int(out, startX);
        writePK2Int(out, startY);
        writePK2Int(out, width);
        writePK2Int(out, height);
    } else { // No compression
        startX = 0;
        startY = 0;
        width = layerWidth;
        height = layerHeight;
    }

    for (int y = startY; y <= startY + height; ++y) {
        for (int x = startX; x <= startX + width; ++x) {
            unsigned char tile = layer[x * y + width];

            out.write(reinterpret_cast<char*>(&tile), sizeof(tile));
        }
    }
}

void PK2FileUtil::writeJsonAsCbor(std::ofstream& out, const nlohmann::json& json) {
    std::vector<std::uint8_t> cborData = nlohmann::json::to_cbor(json);
    uint64_t size = cborData.size();
    out.write(reinterpret_cast<char*>(&size), sizeof(size));
    out.write(reinterpret_cast<char*>(cborData.data()), size);
}
