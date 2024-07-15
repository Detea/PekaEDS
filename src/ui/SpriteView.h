#pragma once
#include <qwidget.h>
#include <qpushbutton.h>
#include <qlineedit.h>
#include <qscrollarea.h>
#include <vector>

#include "LegacySpriteFileReader.h"
#include "SpriteListView.h"
#include "ImageCache.h"

#include "Region.h"

#include "GamePaths.h"
#include "ToolMode.h"

class SpriteView : public QWidget {
    Q_OBJECT

public:
	SpriteView(QWidget* parent);

	void setLegacySpriteReader(LegacySpriteFileReader* reader);
    void setImageCache(ImageCache& cache);

    void setGamePaths(const GamePaths* paths);

    void setSelectedSprite();

public slots:
	void setSpriteData(std::vector<QString>* spriteFilenames, std::vector<PK2::SpriteBase>* spriteList);

    // For internal use only. Do not connect to this slot!
    void spriteListSelectionChanged(int newSelection);

signals:
    void spritesAdded(const QStringList& filePaths);
    void spriteRemoved(int spriteListIndex);
    void spriteReplaced(int originalSpriteIndex, const QString& newSpriteFilePath);
    void spriteSetAsPlayer(int newPlayerIndex);

    void spriteMoved(int originalPosition, int newPosition);

    void selectionChanged(int newSelection);

    void selectedRegionChanged(int selectedIndex);
    void regionsLoaded(const std::vector<Region>* regionList);

    void modeChanged(ToolMode mode);

private slots:
    void addSpriteSlot();
    void removeSpriteSlot();
    void replaceSpriteSlot();
    void setPlayerSlot();

	void clearSearch();

    void executeSearch(const QString& text);

private:
	void setupUI();
	void connectSlots();

private:
	QPushButton* btnAdd = nullptr;
	QPushButton* btnRemove = nullptr;
	QPushButton* btnReplace = nullptr;
	QPushButton* btnSetPlayer = nullptr;

	QLineEdit* leSearch = nullptr;
	QPushButton* btnClear = nullptr;

	QPushButton* btnFilter = nullptr;

	QScrollArea* scrollArea = nullptr;
	SpriteListView* spriteListView = nullptr;
	ImageCache* spriteImageCache = nullptr;

	LegacySpriteFileReader* legacySpriteReader = nullptr;

	std::vector<QString>* spriteFilenames = nullptr;
	std::vector<PK2::SpriteBase>* spriteList = nullptr;

    std::vector<PK2::SpriteBase> searchDisplayList;

    const GamePaths* gamePaths = nullptr;

    bool searching = false;
};

