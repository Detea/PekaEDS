#include "EDSMainMenu.h"

EDSMainMenu::EDSMainMenu() {
	setupMenus();
}

void EDSMainMenu::setupMenus() {
	mFile = addMenu("File");

	mFileNew = new QAction("New", this);
    mFileLoad = new QAction("Load", this);
    mFileSave = new QAction("Save", this);
    mFileSaveAs = new QAction("Save As...", this);

	mFile->addAction(mFileNew);
    mFile->addAction(mFileLoad);
    mFile->addAction(mFileSave);
    mFile->addAction(mFileSaveAs);
}
