#pragma once
#include <qtoolbar.h>
#include <qcombobox.h>

class EDSToolBar : public QToolBar {
    friend class PekaEDSWindow;

public:
	EDSToolBar(QWidget* parent);

    void setSelectedLayer(int index);

private slots:
    void changeMode(int newMode);

private:
	void setupUI();

private:
	QAction* actnNew = nullptr;
	QAction* actnLoad = nullptr;
	QAction* actnSave = nullptr;
	QAction* actnSaveAs = nullptr;

	QAction* actnHighlightSprites = nullptr;
    QAction* actnShowSpriteSize = nullptr;
	QAction* actnShowForegroundSprites = nullptr;
	QAction* actnShowBackgroundSprites = nullptr;

	QComboBox* cbLayers = nullptr;
    QComboBox* cbMode = nullptr;
};

