#include "Settings.h"

#include <fstream>
#include <QDebug>

Settings::Settings() {}

bool Settings::load(const std::string& filename) {
    std::ifstream in(filename, std::ios::binary);

    if (!in) {
        return false;
    }

    int strLen = 0;
    in.read(reinterpret_cast<char*>(&strLen), sizeof(strLen));

    char* strGamePath = new char[strLen];
    in.read(strGamePath, strLen * sizeof(char));

    in.read(reinterpret_cast<char*>(&highlightSprites), sizeof(highlightSprites));
    in.read(reinterpret_cast<char*>(&showSpriteSize), sizeof(showSpriteSize));

    if (!in.good()) {
        return false;
    }

    gameDirectory = QString::fromStdString(strGamePath);

    delete[] strGamePath;

    return true;
}

// TODO [IMPROVEMENT] Should it really fail silently? Maybe return something and inform the user
void Settings::save(const std::string& filename) {
    if (!gameDirectory.isEmpty()) {
        if (std::ofstream out(filename, std::ios::binary); out) {
            int strLen = gameDirectory.size();
            out.write(reinterpret_cast<char*>(&strLen), sizeof(strLen));

            std::string strGamePath = gameDirectory.toStdString();
            out.write(reinterpret_cast<char*>(strGamePath.data()), strLen * sizeof(char));

            out.write(reinterpret_cast<char*>(&highlightSprites), sizeof(highlightSprites));
            out.write(reinterpret_cast<char*>(&showSpriteSize), sizeof(showSpriteSize));

            out.flush();
            out.close();
        }
    }
}

void Settings::setHighlightSprites(bool highlight) {
    highlightSprites = highlight;
}

bool Settings::getHighlightSprites() {
    return highlightSprites;
}

void Settings::setShowSpriteSize(bool showSize) {
    showSpriteSize = showSize;
}

bool Settings::getShowSpriteSize() {
    return showSpriteSize;
}

void Settings::setGameDirectory(const QString& path) {
    gameDirectory = path;
}

const QString& Settings::getGameDirectory() {
    return gameDirectory;
}
