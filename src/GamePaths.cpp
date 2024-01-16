#include "GamePaths.h"
#include <qdebug.h>

GamePaths::GamePaths() {

}

void GamePaths::setBasePath(std::string_view path) {
	const char separator = std::filesystem::path::preferred_separator; 

	basePath = path;
	basePath += separator;

	tilesetPath = basePath;
	tilesetPath += "gfx";
	tilesetPath += separator;
	tilesetPath += "tiles";
	tilesetPath += separator;

	backgroundPath = basePath;
	backgroundPath += "gfx";
	backgroundPath += separator;
	backgroundPath += "scenery";
	backgroundPath += separator;

	musicPath = basePath;
	musicPath += "music";
	musicPath += separator;

	spritePath = basePath;
	spritePath += "sprites";
	spritePath += separator;

	episodePath = basePath;
	episodePath += "episodes";
	episodePath += separator;
}

std::string_view GamePaths::getTilesetPath() const {
	return tilesetPath;
}

std::string_view GamePaths::getBackgroundPath() const {
	return backgroundPath;
}

std::string_view GamePaths::getMusicPath() const {
	return musicPath;
}

std::string_view GamePaths::getSpritePath() const {
	return spritePath;
}

std::string_view GamePaths::getEpisodesFolderPath() const {
	return episodePath;
}

std::string GamePaths::getTilesetFileEpisode(std::string_view loadedEpisode, std::string_view filename) const {
	const char separator = std::filesystem::path::preferred_separator;

	// TODO is this correct?
	return episodePath + separator + loadedEpisode.data() + separator + filename.data();
}

std::string GamePaths::getBackgroundFileEpisode(std::string_view loadedEpisode, std::string_view filename) const {
	const char separator = std::filesystem::path::preferred_separator;

	// TODO is this correct?
	return episodePath + separator + loadedEpisode.data() + separator + filename.data();
}

std::string GamePaths::getTilesetFile(std::string_view filename) const {
	// TODO Maybe pass a const std::string&? I don't like this. But this code only gets called when the user loads a new map or tileset, so it should be fine.
	return tilesetPath + filename.data();
}

std::string GamePaths::getBackgroundFile(std::string_view filename) const {
	// TODO Maybe pass a const std::string&? I don't like this. But this code only gets called when the user loads a new map or tileset, so it should be fine.
	return backgroundPath + filename.data();
}