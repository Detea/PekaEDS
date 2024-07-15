#include "SpriteView.h"
#include <qboxlayout.h>
#include <qlabel.h>
#include <qfiledialog.h>

SpriteView::SpriteView(QWidget* parent) : QWidget(parent) {
	setupUI();
	connectSlots();
}

void SpriteView::setupUI() {
	btnAdd = new QPushButton("Add");
	btnRemove = new QPushButton("Remove");
	btnReplace = new QPushButton("Replace");
	btnSetPlayer = new QPushButton("Set Player");

	QHBoxLayout* hboxTop = new QHBoxLayout;
	hboxTop->setContentsMargins(2, 0, 2, 0);
	hboxTop->addWidget(btnAdd);
	hboxTop->addWidget(btnRemove);
	hboxTop->addWidget(btnReplace);
	hboxTop->addWidget(btnSetPlayer);

	QLabel* lblSearch = new QLabel("Search:");
	leSearch = new QLineEdit;
	btnClear = new QPushButton("Clear");
	btnClear->setFixedWidth(40);

	btnFilter = new QPushButton("Filter");

	QHBoxLayout* hboxSearch = new QHBoxLayout;
	hboxSearch->addWidget(lblSearch);
	hboxSearch->addWidget(leSearch);
	hboxSearch->addWidget(btnClear);
    //hboxSearch->addStretch();
    //hboxSearch->addWidget(btnFilter);

	spriteListView = new SpriteListView;
	scrollArea = new QScrollArea(this);
	scrollArea->setWidget(spriteListView);
	scrollArea->setWidgetResizable(true);

	QVBoxLayout* mainLayout = new QVBoxLayout;
	mainLayout->setContentsMargins(0, 0, 0, 0);
	mainLayout->addLayout(hboxTop);
	mainLayout->addLayout(hboxSearch);
    mainLayout->addWidget(scrollArea);

	setLayout(mainLayout);
}

void SpriteView::connectSlots() {
    // TODO Connect buttons to the signals
    connect(btnAdd, &QPushButton::pressed, this, &SpriteView::addSpriteSlot);

    connect(btnRemove, &QPushButton::pressed, this, &SpriteView::removeSpriteSlot);
    connect(btnReplace, &QPushButton::pressed, this, &SpriteView::replaceSpriteSlot);
    connect(btnSetPlayer, &QPushButton::pressed, this, &SpriteView::setPlayerSlot);

    connect(leSearch, &QLineEdit::textEdited, this, &SpriteView::executeSearch);
	connect(btnClear, &QPushButton::pressed, this, &SpriteView::clearSearch);

    connect(spriteListView, &SpriteListView::selectedSpriteChanged, this, &SpriteView::spriteListSelectionChanged);
    connect(this, &SpriteView::selectedRegionChanged, spriteListView, &SpriteListView::selectedRegionChanged);
    connect(this, &SpriteView::regionsLoaded, spriteListView, &SpriteListView::regionsLoaded);
}

void SpriteView::spriteListSelectionChanged(int newSelection) {
    int selection = newSelection;

    // Need to find the index of the sprite in the list when searching, because the search list gets displayed
    if (searching) {
        for (int i = 0; i < spriteList->size(); ++i) {
            if (spriteList->at(i).filename == searchDisplayList[newSelection].filename) {
                selection = i;

                break;
            }
        }
    }

    emit selectionChanged(selection);
}

void SpriteView::executeSearch(const QString& searchText) {
    searchDisplayList.clear();

    if (searchText.isEmpty()) {
        spriteListView->setSpriteList(spriteList);
        spriteListView->setAllowSpriteReordering(true);

        btnReplace->setEnabled(true);
        btnRemove->setEnabled(true);
        btnSetPlayer->setEnabled(true);

        searching = false;
    } else {
        if (!searching) {
            searching = true;

            spriteListView->clearSelection();
            spriteListView->setAllowSpriteReordering(false);

            btnReplace->setEnabled(false);
            btnRemove->setEnabled(false);
            btnSetPlayer->setEnabled(false);
        }

        for (PK2::SpriteBase& sprite : *spriteList) {
            qsizetype filenameResult = sprite.filename.indexOf(searchText);
            qsizetype nameResult = sprite.name.indexOf(searchText);

            if (filenameResult != -1 || nameResult != -1) {
                searchDisplayList.push_back(sprite);
            }
        }

        spriteListView->setSpriteList(&searchDisplayList);
        spriteListView->update();

        /*
         *  TODO find out how to obtain default text color
        else {
            leSearch->setStyleSheet("color: red;");
        }
        */
    }
}

void SpriteView::addSpriteSlot() {
    QStringList spriteFiles = QFileDialog::getOpenFileNames(this, "Add a sprite...", gamePaths->spriteFolder(), "Pekka Kana 2 sprite file (*.spr *.spr2)");

	if (!spriteFiles.isEmpty()) {
        emit spritesAdded(spriteFiles);

        spriteListView->updateList();
		scrollArea->ensureVisible(0, spriteListView->height());
	}
}


void SpriteView::replaceSpriteSlot() {
    int selectedIndex = spriteListView->getSelectedIndex();

    if (selectedIndex != -1) {
        QString spriteFile = QFileDialog::getOpenFileName(this, "Select a new sprite for replacement...", gamePaths->spriteFolder(), "Pekka Kana 2 sprite file (*.spr *.spr2)");

        if (!spriteFile.isEmpty()) {
            spriteListView->update();

            emit spriteReplaced(selectedIndex, spriteFile);
        }
    }
}

void SpriteView::setPlayerSlot() {
    int selectedIndex = spriteListView->getSelectedIndex();

	if (selectedIndex != -1) {
        emit spriteSetAsPlayer(selectedIndex);

        spriteListView->update();
    }
}

void SpriteView::removeSpriteSlot() {
    int selectedIndex = spriteListView->getSelectedIndex();

    if (selectedIndex != -1) {
        if (selectedIndex >= spriteList->size() - 1) spriteListView->setSelectedIndex(selectedIndex - 1);

        spriteListView->updateList();

        emit spriteRemoved(selectedIndex);
    }
}

void SpriteView::clearSearch() {
    searchDisplayList.clear();
    spriteListView->setSpriteList(spriteList);
    spriteListView->clearSelection();
    spriteListView->setAllowSpriteReordering(true);
    spriteListView->update();

    searching = false;

	leSearch->setText("");

    btnReplace->setEnabled(true);
    btnRemove->setEnabled(true);
    btnSetPlayer->setEnabled(true);
}

void SpriteView::setSpriteData(std::vector<QString>* newSpriteFileList, std::vector<PK2::SpriteBase>* newSpriteList) {
	spriteFilenames = newSpriteFileList;

	spriteList = newSpriteList;

    spriteListView->setSpriteList(spriteList);
}

void SpriteView::setLegacySpriteReader(LegacySpriteFileReader* reader) {
	legacySpriteReader = reader;
}

void SpriteView::setImageCache(ImageCache& cache) {
    spriteImageCache = &cache;

    spriteListView->setImageCache(cache);
}

void SpriteView::setGamePaths(const GamePaths* paths) {
    gamePaths = paths;
}
