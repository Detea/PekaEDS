#pragma once
#include "PK2LegacyMapData.h"
#include "PK2MapBaseMetadata.h"
#include "Region.h"
#include "GamePaths.h"

#include <vector>
#include <nlohmann/json.hpp>

/*
 * FileHandler
 *
 * This class is responsible for, and only for this, reading map and sprite data from files.
 *
 * Images, regions, etc. are loaded in the MapHandler class.
 */
class FileHandler {
public:
    FileHandler();

    int readMapVersion13(const QString& file, PK2::LegacyMapData& legacyMapData);
    int readMapVersion15(const QString& file, PK2::MapBaseMetadata& metadata, std::vector<Region>& regions);

    int saveMapVersion15(const QString& file, const PK2::MapBaseMetadata& metadata, const std::vector<Region>& regions);

    /*
     * This function returns the following values:
     * 0 - On success
     * 1 - If it failed to open the file for reading
     * 2 - If the file that's provided is of the wrong format
     */

    // TODO ErrorList or store all errors in this class in a list instead of a string?
    // Reads in only the needed information
    int readSpriteVersion13(const QString& file, PK2::SpriteBase& targetSprite);
    int readSpriteVersion15(const QString& file, PK2::SpriteBase& targetSprite, std::vector<QString>& errorList);

    void setGamePaths(const GamePaths* gamePaths);

    const QString& lastError();

private:
    // If isParentJson is false this method will not create errors and set default values
    void setSprite15JSONData(PK2::SpriteBase& targetSprite, const QString filePath, std::vector<QString>& errorList, nlohmann::json& json, nlohmann::json& parentJson);
    void setColorJSON(PK2::SpriteBase& targetSprite, const QString filePath, std::vector<QString>& errorList, nlohmann::json& json);
    void setFrameDataJSON(PK2::SpriteBase& targetSprite, const QString filePath, std::vector<QString>& errorList, nlohmann::json& json);

public:
    static constexpr int SUCCESS = 0;
    static constexpr int ERROR_FILE_NOT_FOUND = 1;
    static constexpr int ERROR_WRONG_VERSION = 2;

private:
    static constexpr unsigned char ID_1_1[] = { 0x31, 0x2E, 0x31, 0x00 };
    static constexpr unsigned char ID_1_2[] = { 0x31, 0x2E, 0x32, 0x00 };
    static constexpr unsigned char ID_1_3[] = { 0x31, 0x2E, 0x33, 0x00 };

    QString lastErrorString;

    const GamePaths* gamePaths = nullptr;
};
