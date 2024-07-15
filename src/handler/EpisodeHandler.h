#pragma once
#include <string>
#include <string_view>
#include <vector>

class EpisodeHandler {
public:
	EpisodeHandler();

	void setEpisodeFolder(std::string_view path);

	std::string_view episodePath() const;

	bool hasEpisodeLoaded() const;

private:
	void loadFiles();

private:
	std::string episodeFolderName;

};