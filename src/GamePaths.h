#pragma once
#include <string>
#include <string_view>

#include <filesystem>

/*
	This class contains the path to the various content folders of the game as strings.

	Each string that can be retrievied with getXXXPath() ends with the paltform specific file separator character.
*/
class GamePaths {
public:
	GamePaths();

	void setBasePath(std::string_view path);

	std::string_view getTilesetPath() const;
	std::string_view getBackgroundPath() const;

	std::string_view getMusicPath() const;

	std::string_view getSpritePath() const;
	
	std::string_view getEpisodesFolderPath() const;

	std::string getTilesetFileEpisode(std::string_view loadedEpisode, std::string_view filename) const;
	std::string getBackgroundFileEpisode(std::string_view loadedEpisode, std::string_view filename) const;

	std::string getTilesetFile(std::string_view filename) const;
	std::string getBackgroundFile(std::string_view filename) const;

private:
	std::string basePath;

	std::string tilesetPath;
	std::string backgroundPath;

	std::string musicPath;

	std::string spritePath;

	std::string episodePath;
};

