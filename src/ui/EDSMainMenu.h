#pragma once
#include <qmenubar.h>
#include <qaction.h>
#include <qmenu.h>

class EDSMainMenu : public QMenuBar {
	friend class PekaEDSWindow;

public:
	EDSMainMenu();

private:
	void setupMenus();

private:
	QMenu* mFile = nullptr;
	QAction* mFileNew = nullptr;
    QAction* mFileLoad = nullptr;
    QAction* mFileSave = nullptr;
    QAction* mFileSaveAs = nullptr;

	// TODO Add options to restore docks
};

