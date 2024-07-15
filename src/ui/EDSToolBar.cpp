#include "EDSToolBar.h"

#include <qlabel.h>
#include "Tool.h"

EDSToolBar::EDSToolBar(QWidget* parent) : QToolBar(parent) {
	setupUI();
}

void EDSToolBar::setSelectedLayer(int index) {
    Q_ASSERT(index >= 0 && index <= 2);

    cbLayers->setCurrentIndex(index);
}

void EDSToolBar::changeMode(int newMode) {
    if (newMode == 0) {
        Tool::setMode(Tool::MODE::TILE);
    } else if (newMode == 1) {
        Tool::setMode(Tool::MODE::SPRITE);
    }
}

void EDSToolBar::setupUI() {
	actnNew = addAction("New");
	actnLoad = addAction("Load");
	actnSave = addAction("Save");
	actnSaveAs = addAction("Save As...");

	// TODO Store return value? Does this leak?
	addSeparator();

	// Sprite visibility control

	actnHighlightSprites = addAction("Highlight");
    actnShowSpriteSize = addAction("Sprite size");
	actnShowForegroundSprites = addAction("Show Foreground");
	actnShowBackgroundSprites = addAction("Show Background");

	actnHighlightSprites->setCheckable(true);
    actnShowSpriteSize->setCheckable(true);
	actnShowForegroundSprites->setCheckable(true);
	actnShowBackgroundSprites->setCheckable(true);

	actnHighlightSprites->setChecked(true);
    actnShowSpriteSize->setChecked(true);
	actnShowForegroundSprites->setChecked(true);
	actnShowBackgroundSprites->setChecked(true);

	addSeparator();

	// Layer control

	QLabel* lblLayer = new QLabel("Layer:", this);

	cbLayers = new QComboBox(this);
	cbLayers->addItem("Both");
	cbLayers->addItem("Foreground");
	cbLayers->addItem("Background");

    QLabel* lblMode = new QLabel("Mode:", this);
    cbMode = new QComboBox(this);
    cbMode->addItem("Tile");
    cbMode->addItem("Sprite");

    connect(cbMode, &QComboBox::currentIndexChanged, this, &EDSToolBar::changeMode);

	addWidget(lblLayer);
	addWidget(cbLayers);

    addSeparator();

    addWidget(lblMode);
    addWidget(cbMode);
}
