#pragma once
#include <qmenubar.h>
#include <qaction.h>
#include <qmenu.h>

class EDSMainMenu : public QMenuBar {
public:
	EDSMainMenu();

	const QAction* const getFileNewAction() const;
	const QAction* const getFileLoadAction() const;
private:
	void setupMenus();

private:
	QMenu* mFile = nullptr;
	QAction* mfNew = nullptr;
	QAction* mfLoad = nullptr;

	// TODO Add options to restore docks
};

