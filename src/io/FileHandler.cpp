#include "FileHandler.h"
#include "PK2FileUtil.h"
#include "LegacyMapConstants.h"

#include "Region.h"

#include <fstream>
#include <QFileInfo>
#include <QFile>

#include <algorithm>

#include <nlohmann/json.hpp>

//#include <QCborStreamReader>

#define READ_JSON_STRING(jsonSource, name) QString::fromStdString(jsonSource[name].get<std::string>())
#define READ_JSON_INT(jsonSource, name) jsonSource[name].get<int>()

#define SPRITE_ERROR(spriteFilename, errorMessage) errorList.push_back("[" + spriteFilename + "] " + errorMessage);
#define SPRITE_MISSING_PROPERTY(spriteFilename, property, defaultValue) \
    errorList.push_back("[" + spriteFilename + "]:\n" +\
    "Sprite does not contain property: '" +\
    property +\
    "'.\nUsing default value: " + defaultValue + ".");

#define SPRITE_MISSING_FRAME(sprite) \
    sprite.imageFile = "missing";\
    sprite.frameX = 0; \
    sprite.frameY = 0; \
    sprite.frameWidth = 32; \
    sprite.frameHeight = 32; \

#define SET_JSON_VALUE_PARENT_STR(value, spriteProperty, childJson, parentJson, missingValue) \
    if (childJson.contains(value)) {\
        targetSprite.spriteProperty = QString::fromStdString(childJson[value].get<std::string>());\
    } else if (parentJson && parentJson->contains(value)) {\
        targetSprite.spriteProperty = QString::fromStdString(parentJson->at(value).get<string>());\
    } else {\
        targetSprite.spriteProperty = missingValue;\
\
        SPRITE_MISSING_PROPERTY(filePath, value, missingValue)\
    }\

FileHandler::FileHandler() {}

int FileHandler::readMapVersion13(const QString& file, PK2::LegacyMapData& legacyMapData) {
    std::ifstream in(file.toStdString().c_str(), std::ios::in | std::ios::binary);

    if (!in) {
        lastErrorString = "Unable to open map file (" + file + ") for reading!";

        return ERROR_FILE_NOT_FOUND;
    }

    unsigned char versionStr[5];

    in.read(reinterpret_cast<char*>(&versionStr), sizeof(versionStr));

    if (!PK2FileUtil::versionMatch(versionStr, Map::LegacyMapConstants::ID)) {
        lastErrorString = "Wrong map format. Expected version 1.3!";

        return ERROR_WRONG_VERSION;
    } else {
        using namespace Map;

        legacyMapData.tilesetFile = PK2FileUtil::readString(in, LegacyMapConstants::FILE_CHARACTER_LIMIT).toLower();
        legacyMapData.backgroundFile = PK2FileUtil::readString(in, LegacyMapConstants::FILE_CHARACTER_LIMIT).toLower();
        legacyMapData.musicFile = PK2FileUtil::readString(in, LegacyMapConstants::FILE_CHARACTER_LIMIT);

        legacyMapData.name = PK2FileUtil::readString(in, LegacyMapConstants::AUTHOR_MAP_NAME_LENGTH);
        legacyMapData.author = PK2FileUtil::readString(in, LegacyMapConstants::AUTHOR_MAP_NAME_LENGTH);

        legacyMapData.levelNumber = PK2FileUtil::readPK2Int(in);
        legacyMapData.weatherType = static_cast<PK2::Weather>(PK2FileUtil::readPK2Int(in));

        // switch values
        PK2FileUtil::readPK2Int(in);
        PK2FileUtil::readPK2Int(in);
        PK2FileUtil::readPK2Int(in);

        legacyMapData.time = PK2FileUtil::readPK2Int(in);

        // "Extra" - not used.
        PK2FileUtil::readPK2Int(in);

        legacyMapData.scrollingType = static_cast<PK2::Scrolling>(PK2FileUtil::readPK2Int(in));

        legacyMapData.playerSpriteIndex = PK2FileUtil::readPK2Int(in);

        legacyMapData.mapX = PK2FileUtil::readPK2Int(in);
        legacyMapData.mapY = PK2FileUtil::readPK2Int(in);

        legacyMapData.iconNumber = PK2FileUtil::readPK2Int(in);

        legacyMapData.spritesAmount = PK2FileUtil::readPK2Int(in);

        legacyMapData.spriteFilenames.clear();
        legacyMapData.spriteFilenames.reserve(legacyMapData.spritesAmount);
        for (int i = 0; i < legacyMapData.spritesAmount; ++i) {
            legacyMapData.spriteFilenames.push_back(PK2FileUtil::readString(in, LegacyMapConstants::FILE_CHARACTER_LIMIT));
        }

        legacyMapData.backgroundLayer.resize(LegacyMapConstants::MAP_SIZE, 255); // background layer
        legacyMapData.foregroundLayer.resize(LegacyMapConstants::MAP_SIZE, 255); // foreground layer

        PK2FileUtil::readLayer(in, legacyMapData.backgroundLayer, LegacyMapConstants::MAP_WIDTH, LegacyMapConstants::MAP_HEIGHT, PK2FileUtil::Compression::LEGACY);
        PK2FileUtil::readLayer(in, legacyMapData.foregroundLayer, LegacyMapConstants::MAP_WIDTH, LegacyMapConstants::MAP_HEIGHT, PK2FileUtil::Compression::LEGACY);

        legacyMapData.spriteLayer.reserve(LegacyMapConstants::MAP_SIZE);

        for (int i = 0; i < LegacyMapConstants::MAP_SIZE; ++i) {
            legacyMapData.spriteLayer.push_back(255);
        }

        PK2FileUtil::readLayer(in, legacyMapData.spriteLayer, LegacyMapConstants::MAP_WIDTH, LegacyMapConstants::MAP_HEIGHT, PK2FileUtil::Compression::LEGACY); // TODO Why 0 elements??? (07.06.24) Wtf did I mean by this? 0 elements?
    }

    lastErrorString = "";
    return SUCCESS;
}

int FileHandler::readMapVersion15(const QString& mapFile, PK2::MapBaseMetadata& mapData, std::vector<Region>& regionList) {
    constexpr char id15Str[5] = {'1', '.', '5', '\0', '\0'};

    std::ifstream in(mapFile.toStdString());
    if (!in) {
        lastErrorString = "Unable to open map file (" + mapFile + ") for reading!";

        return ERROR_FILE_NOT_FOUND;
    }

    char readId[5];
    in.read(readId, sizeof(readId));

    if (!std::equal(std::begin(readId), std::end(readId), std::begin(id15Str))) {
        lastErrorString = "Wrong map format. Expected version 1.5!";

        return ERROR_WRONG_VERSION;
    }

    /*
     * To read map:
     * read id (5 bytes)
     *
     * Read level header:
     * read size of cbor contents (8 bytes)
     * read cbor data
     */
    std::uint64_t headerSize;
    in.read(reinterpret_cast<char*>(&headerSize), sizeof(headerSize));

    std::vector<std::uint8_t> headerData;
    headerData.resize(headerSize);

    in.read(reinterpret_cast<char*>(headerData.data()), headerData.size() / sizeof(std::uint8_t));

    // Read map header
    nlohmann::json json = nlohmann::json::from_cbor(headerData);
    mapData.author = READ_JSON_STRING(json, "author");
    mapData.name = READ_JSON_STRING(json, "name");
    mapData.levelNumber = READ_JSON_INT(json, "level_number");
    mapData.iconNumber = READ_JSON_INT(json, "icon_id");
    mapData.mapX = READ_JSON_INT(json, "icon_x");
    mapData.mapY = READ_JSON_INT(json, "icon_y");

    std::vector<std::string> spriteFiles = json["sprite_prototypes"].get<std::vector<std::string>>();
    for (const std::string& str : spriteFiles) {
        mapData.spriteFilenames.push_back(QString::fromStdString(str));
    }

    mapData.playerSpriteIndex = READ_JSON_INT(json, "player_index");
    mapData.time = READ_JSON_INT(json, "map_time");
    mapData.luaScript = READ_JSON_STRING(json, "lua_script");
    if (json.contains("game_mode")) mapData.gameMode = READ_JSON_INT(json, "game_mode");

    // Read regions
    int numRegions = READ_JSON_INT(json, "regions");
    for (int i = 0; i < numRegions; ++i) {
        // Setup the cbor data vector
        std::uint64_t regionDataSize;
        in.read(reinterpret_cast<char*>(&regionDataSize), sizeof(regionDataSize));

        std::vector<std::uint8_t> regionDataVec;
        regionDataVec.resize(regionDataSize);

        in.read(reinterpret_cast<char*>(regionDataVec.data()), regionDataSize);

        // Read the region data
        nlohmann::json regionJson = nlohmann::json::from_cbor(regionDataVec);

        Region region;
        if (regionJson.contains("name")) {
            region.editorName = READ_JSON_STRING(regionJson, "name");
        } else {
            region.editorName = "Region #" + QString::number(regionList.size() + 1);
        }

        region.width = READ_JSON_INT(regionJson, "width");
        region.height = READ_JSON_INT(regionJson, "height");
        region.compression = READ_JSON_INT(regionJson, "compression");
        region.tilesetImageFile = READ_JSON_STRING(regionJson, "tileset").toLower();

        if (regionJson.contains("tileset_bg")) {
            region.tilesetBackgroundImageFile = READ_JSON_STRING(regionJson, "tileset_bg");
        }

        region.backgroundImageFile = READ_JSON_STRING(regionJson, "background").toLower();

        region.musicFile = READ_JSON_STRING(regionJson, "music");

        region.scrolling = READ_JSON_INT(regionJson, "scrolling");
        region.weather = READ_JSON_INT(regionJson, "weather");
        region.splashColor = READ_JSON_INT(regionJson, "splash_color");
        region.fireColor1 = READ_JSON_INT(regionJson, "fire_color_1");
        region.fireColor2 = READ_JSON_INT(regionJson, "fire_color_2");

        // Read layers
        int regionSize = region.width * region.height;

        region.backgroundLayer.resize(regionSize, 255);
        PK2FileUtil::readLayer(in, region.backgroundLayer, region.width, region.height, region.compression);

        region.foregroundLayer.resize(regionSize, 255);
        PK2FileUtil::readLayer(in, region.foregroundLayer, region.width, region.height, region.compression);

        region.spriteLayer = std::vector(regionSize, 255);
        PK2FileUtil::readLayer(in, region.spriteLayer, region.width, region.height, region.compression);

        regionList.push_back(region);
    }

    return SUCCESS;
}

int FileHandler::saveMapVersion15(const QString& filePath, const PK2::MapBaseMetadata& metadata, const std::vector<Region>& regions) {
    std::ofstream out(filePath.toStdString());

    if (!out) {
        // TODO [IMPROVEMENT] It would be better to use a logger
        lastErrorString = "Unable to open file '" + filePath + "' for writing!";

        return ERROR_FILE_NOT_FOUND;
    }

    constexpr char version[5] = "1.5";
    out.write(version, sizeof(version));

    // Write metadata
    nlohmann::json json;
    json["name"] = metadata.name.toStdString();
    json["author"] = metadata.author.toStdString();
    json["level_number"] = metadata.levelNumber;
    json["icon_id"] = metadata.iconNumber;
    json["icon_x"] = metadata.mapX;
    json["icon_y"] = metadata.mapY;

    std::vector<std::string> spriteFilenames;
    for (const QString& str : metadata.spriteFilenames) {
        spriteFilenames.push_back(str.toStdString());
    }

    json["sprite_prototypes"] = spriteFilenames;
    json["player_index"] = metadata.playerSpriteIndex;

    json["regions"] = regions.size();
    json["map_time"] = metadata.time;
    json["lua_script"] = metadata.luaScript.toStdString();
    json["game_mode"] = metadata.gameMode;

    PK2FileUtil::writeJsonAsCbor(out, json);

    // Write regions
    for (const Region& region : regions) {
        nlohmann::json regionJson;

        regionJson["width"] = region.width;
        regionJson["height"] = region.height;
        regionJson["compression"] = region.compression;
        regionJson["tileset"] = region.tilesetImageFile.toStdString();

        if (!region.tilesetBackgroundImageFile.isEmpty()) {
            regionJson["tileset_bg"] = region.tilesetBackgroundImageFile.toStdString();
        }

        regionJson["music"] = region.musicFile.toStdString();
        regionJson["background"] = region.backgroundImageFile.toStdString();
        regionJson["scrolling"] = region.scrolling;
        regionJson["weather"] = region.weather;

        regionJson["splash_color"] = region.splashColor;
        regionJson["fire_color_1"] = region.fireColor1;
        regionJson["fire_color_2"] = region.fireColor2;
        regionJson["name"] = region.editorName.toStdString();

        PK2FileUtil::writeJsonAsCbor(out, regionJson);

        PK2FileUtil::writeLayer(out, region.backgroundLayer, region.width, region.height, region.compression);
        PK2FileUtil::writeLayer(out, region.foregroundLayer, region.width, region.height, region.compression);
        PK2FileUtil::writeLayer(out, region.spriteLayer, region.width, region.height, region.compression);
    }

    out.flush();
    out.close();

    return SUCCESS;
}

int FileHandler::readSpriteVersion13(const QString& filename, PK2::SpriteBase& sprite) {
    std::ifstream in(filename.toStdString().c_str(), std::ios::in | std::ios::binary);

    if (!in) {
        lastErrorString = "Unable to open sprite file: " + filename;

        return ERROR_FILE_NOT_FOUND;
    }

    unsigned char version[4];

    in.read(reinterpret_cast<char*>(&version), sizeof(version));

    char sprVersion = 0;

    if (std::equal(std::begin(version), std::end(version), std::begin(ID_1_1))) sprVersion = 11;
    if (std::equal(std::begin(version), std::end(version), std::begin(ID_1_2))) sprVersion = 12;
    if (std::equal(std::begin(version), std::end(version), std::begin(ID_1_3))) sprVersion = 13;

    if (sprVersion != 11 && sprVersion != 12 && sprVersion != 13) {
        // All custom and most official sprites are at least version 1.3, so we expect that one for legacy sprites
        lastErrorString = "Sprite is not of format 1.1, 1.2 or 1.3!";

        return ERROR_WRONG_VERSION;
    }

    sprite.filename = QFileInfo(filename).fileName();

    int discardedInt = 0;
    unsigned char discardedByte;
    bool discardedBool;

    // Used - Sprite type
    in.read(reinterpret_cast<char*>(&sprite.type), sizeof(sprite.type));

    // Used - Sprites image sheet
    if (sprVersion == 11 || sprVersion == 12) {
        sprite.imageFile = PK2FileUtil::readString(in, 13).toLower();
    } else if (sprVersion == 13) {
        sprite.imageFile = PK2FileUtil::readString(in, 100).toLower();
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
    in.read(reinterpret_cast<char*>(&sprite.frameX), sizeof(sprite.frameX));
    in.read(reinterpret_cast<char*>(&sprite.frameY), sizeof(sprite.frameY));

    // Used
    in.read(reinterpret_cast<char*>(&sprite.frameWidth), sizeof(sprite.frameWidth));
    in.read(reinterpret_cast<char*>(&sprite.frameHeight), sizeof(sprite.frameHeight));

    // Unused - Distance between frames
    in.read(reinterpret_cast<char*>(&discardedInt), sizeof(discardedInt));

    // Used
    int nameLength = 32;
    if (sprVersion == 11 || sprVersion == 12) {
        nameLength = 30;
    }

    sprite.name = PK2FileUtil::readString(in, nameLength);

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
    in.read(reinterpret_cast<char*>(&sprite.color), sizeof(sprite.color));

    return 0;
}

int FileHandler::readSpriteVersion15(const QString& filePath, PK2::SpriteBase& targetSprite, std::vector<QString>& errorList) {
    QFile file(filePath);
    if (!file.exists()) {
        SPRITE_ERROR(filePath, "Sprite file doesn't exist!");

        return FileHandler::ERROR_FILE_NOT_FOUND;
    }

    if (!file.open(QIODevice::ReadOnly)) {
        SPRITE_ERROR(filePath, "Unable to open file for reading.")
    }

    nlohmann::json json = nlohmann::json::parse(file.readAll(), nullptr, false);
    if (json.is_discarded()) {
        SPRITE_ERROR(filePath, "Unable to parse JSON!")

        return FileHandler::ERROR_FILE_NOT_FOUND; // TODO Return something else
    }

    if (!json.contains("version")) {
        errorList.push_back("File does not contain property: 'version'.");

        return FileHandler::ERROR_WRONG_VERSION;
    }

    nlohmann::json versionJson = json["version"];
    if (!versionJson.is_string()) {
        errorList.push_back("Property 'version' has to be a string.");

        return FileHandler::ERROR_WRONG_VERSION;
    }

    if (versionJson.get<std::string>() != "2.0") {
        errorList.push_back("Property 'version' is not the required value: '2.0'.");

        return FileHandler::ERROR_WRONG_VERSION;
    }

    nlohmann::json parentJson;
    if (json.contains("parent")) {
        QString parentFile = QString::fromStdString(json["parent"].get<std::string>());
        QString parentFilePath = gamePaths->getSprite(parentFile);

        // Look for parentFile.spr2 if .spr was not found
        if (parentFilePath.endsWith(".spr") && parentFilePath == parentFile) {
            parentFile += "2"; // Append a 2 to get ".spr2"
            parentFilePath = gamePaths->getSprite(parentFile);
        }

        if (parentFilePath == parentFile) {
            SPRITE_ERROR(filePath, "Unable to find parent sprite: '" + parentFilePath + "'.")
        } else {
            QFile file(parentFilePath);
            if (!file.open(QIODevice::ReadOnly)) {
                SPRITE_ERROR(filePath, "Unable to open parent sprite: '" + parentFilePath + "'.")
            } else {
                parentJson = nlohmann::json::parse(file.readAll(), nullptr, false);

                if (parentJson.is_discarded()) {
                    SPRITE_ERROR(filePath, "Unable to parse parent file JSON: '" + parentFile + "'.");
                }
            }
        }
    }

    setSprite15JSONData(targetSprite, filePath, errorList, json, parentJson);
    targetSprite.filename = QFileInfo(filePath).fileName();

    return FileHandler::SUCCESS;
}

void FileHandler::setSprite15JSONData(PK2::SpriteBase& targetSprite, const QString filePath, std::vector<QString>& errorList, nlohmann::json& json, nlohmann::json& parentJson) {
    if (json.contains("picture")) {
        targetSprite.imageFile = QString::fromStdString(json["picture"].get<std::string>());
    } else if(parentJson.contains("picture")) {
        targetSprite.imageFile = QString::fromStdString(parentJson["picture"].get<std::string>());
    } else {
        targetSprite.imageFile = "missing";

        SPRITE_MISSING_PROPERTY(filePath, "picture", "missing image")
    }

    if (json.contains("name")) {
        targetSprite.name = QString::fromStdString(json["name"].get<std::string>());
    } else if (parentJson.contains("name")) {
        targetSprite.name = QString::fromStdString(parentJson["name"].get<std::string>());
    } else {
        targetSprite.name = "Unnamed";

        SPRITE_MISSING_PROPERTY(filePath, "name", "'Unnamed'")
    }

    if (json.contains("color")) {
        setColorJSON(targetSprite, filePath, errorList, json);
    } else if (parentJson.contains("color")) {
        setColorJSON(targetSprite, filePath, errorList, parentJson);
    } else {
        targetSprite.color = 255;

        SPRITE_MISSING_PROPERTY(filePath, "color", "255")
    }

    if (json.contains("type")) {
        targetSprite.type = json["type"].get<int>();
    } else if (parentJson.contains("type")) {
        targetSprite.type = parentJson["type"].get<int>();
    } else {
        SPRITE_MISSING_PROPERTY(filePath, "type", 0)
    }

    if (json.contains("frame")) {
        setFrameDataJSON(targetSprite, filePath, errorList, json);
    } else if (parentJson.contains("frame")) {
        setFrameDataJSON(targetSprite, filePath, errorList, parentJson);
    } else {
        targetSprite.imageFile = "missing";
        targetSprite.frameX = 0;
        targetSprite.frameY = 0;
        targetSprite.frameWidth = 32;
        targetSprite.frameHeight = 32;

        SPRITE_MISSING_PROPERTY(filePath, "frame", "x: 0, y: 0; width: 32, height: 32")
    }
}

void FileHandler::setColorJSON(PK2::SpriteBase& targetSprite, const QString filePath, std::vector<QString>& errorList, nlohmann::json& json) {
    nlohmann::json colorJson = json["color"];

    if (colorJson.is_number()) {
        targetSprite.color = colorJson.get<int>();
    } else if (colorJson.is_string()) {
        static QMap<std::string, int> colorStrings;

        if (colorStrings.empty()) {
            colorStrings["gray"] = 0;
            colorStrings["blue"] = 32;
            colorStrings["red"] = 64;
            colorStrings["green"] = 96;
            colorStrings["orange"] = 128;
            colorStrings["violet"] = 160;
            colorStrings["turquoise"] = 192;
            colorStrings["normal"] = 255;
        }

        std::string color = colorJson.get<std::string>();

        if (colorStrings.contains(color)) {
            targetSprite.color = colorStrings[color];
        } else {
            SPRITE_ERROR(filePath, "Unsupported color: " + QString::fromStdString(color));
        }
    } else {
        SPRITE_ERROR(filePath, "Color property must be an int or a string!");
    }
}

void FileHandler::setFrameDataJSON(PK2::SpriteBase& targetSprite, const QString filePath, std::vector<QString>& errorList, nlohmann::json& json) {
    nlohmann::json frame = json["frame"];

    if (frame.contains("pos_x")) {
        targetSprite.frameX = frame["pos_x"];
    } else {
        SPRITE_MISSING_FRAME(targetSprite)
        SPRITE_MISSING_PROPERTY(filePath, "frame.pos_x", "x: 0, y: 0; width: 32, height: 32")
    }

    if (frame.contains("pos_y")) {
        targetSprite.frameY = frame["pos_y"];
    } else {
        SPRITE_MISSING_FRAME(targetSprite)
        SPRITE_MISSING_PROPERTY(filePath, "frame.pos_y", "x: 0, y: 0; width: 32, height: 32")
    }

    if (frame.contains("width")) {
        targetSprite.frameWidth = frame["width"];
    } else {
        SPRITE_MISSING_FRAME(targetSprite)
        SPRITE_MISSING_PROPERTY(filePath, "frame.width", "x: 0, y: 0; width: 32, height: 32")
    }

    if (frame.contains("height")) {
        targetSprite.frameHeight = frame["height"];
    } else {
        SPRITE_MISSING_FRAME(targetSprite)
        SPRITE_MISSING_PROPERTY(filePath, "frame.height", "x: 0, y: 0; width: 32, height: 32")
    }
}

void FileHandler::setGamePaths(const GamePaths* gamePaths) {
    this->gamePaths = gamePaths;
}

const QString& FileHandler::lastError() {
    return lastErrorString;
}
