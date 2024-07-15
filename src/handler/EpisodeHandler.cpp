#include "EpisodeHandler.h"

EpisodeHandler::EpisodeHandler() {}

void EpisodeHandler::setEpisodeFolder(std::string_view path) {
	episodeFolderName = path;
}

std::string_view EpisodeHandler::episodePath() const {
	return episodeFolderName;
}

bool EpisodeHandler::hasEpisodeLoaded() const {
	return !episodeFolderName.empty();
}