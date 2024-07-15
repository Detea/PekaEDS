#pragma once
#include <string>
#include <QString>

class Settings {
public:
    Settings();

    // Return true if the settings file was loaded, false otherwise. If false the SetGamePath widget will be shown in PekaEDSWindow.
    // These two take an std::string instead of a QString, because ifstream takes an std::string.
    bool load(const std::string& filename);
    void save(const std::string& filename);

    void setHighlightSprites(bool highlight);
    bool getHighlightSprites();

    void setShowSpriteSize(bool showSize);
    bool getShowSpriteSize();

    void setGameDirectory(const QString& path);
    const QString& getGameDirectory();

private:
    // Storing it as QString for convinience, because the GamePaths classs expects a QString.
    QString gameDirectory;

    bool highlightSprites = true;
    bool showSpriteSize = true;
};

