#include "EDSMainMenu.h"

EDSMainMenu::EDSMainMenu() {
	setupMenus();
}

void EDSMainMenu::setupMenus() {
	mFile = addMenu("File");

	mfNew = new QAction("New", this);
	mfLoad = new QAction("Load", this);

	mFile->addAction(mfNew);
	mFile->addAction(mfLoad);
}

const QAction* const EDSMainMenu::getFileNewAction() const {
	return mfNew;
}

const QAction* const EDSMainMenu::getFileLoadAction() const {
	return mfLoad;
}