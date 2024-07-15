#pragma once
#include <qwidget.h>
#include <QPaintEvent>
#include <qevent.h>
#include <qstatictext.h>

#include <vector>

#include "PK2SpriteBase.h"
#include "Region.h"
#include "ImageCache.h"

/*
 * For some reason I decided to implement a custom list component, which ended up being a lot more work than necessary.
 * This component should be rewritten/removed. A QListView with custom QStyledItemDelegate.
 */
class SpriteListView : public QWidget {
	Q_OBJECT

public:
	SpriteListView();

    void setSpriteList(std::vector<PK2::SpriteBase>* newSpriteList);

	int getSelectedIndex();

    void clearSelection();

    void setAllowSpriteReordering(bool allow);

    void spritePixmapListChanged(const std::vector<QPixmap*>* newList);

    void setImageCache(ImageCache& imageCache);
    void updateList();

    void setSelectedIndex(int index);

public slots:
    void selectedRegionChanged(int selectedIndex); // TODO Pass region instead of index
    void regionsLoaded(const std::vector<Region>* regionList);

signals:
	void selectedSpriteChanged(int selection);
    void spriteMoved(int originalPosition, int newPosition);

    void modeChanged(int newMode);

protected:
	void paintEvent(QPaintEvent* event) override;

    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;

private:
	void setupUI();

private:
	// Compact mode shows all sprites in a list
	static constexpr int IMAGE_OFFSET_X = 5;
	static constexpr int IMAGE_OFFSET_Y = 5;

	static constexpr int IMAGE_MAX_WIDTH_COMPACT = 64;
	static constexpr int IMAGE_MAX_HEIGHT_COMPACT = 64;

	static constexpr int ENTRY_WIDTH_COMPACT = 300;
	static constexpr int ENTRY_HEIGHT_COMPACT = (IMAGE_OFFSET_Y * 2) + IMAGE_MAX_HEIGHT_COMPACT;

	static constexpr int NAME_X = IMAGE_MAX_WIDTH_COMPACT + (IMAGE_OFFSET_X * 2);
	static constexpr int NAME_Y = 15;

	// Expanded mode shows sprites by rows and columns
	// TODO Implement expanded mode
	static constexpr int ENTRY_WIDTH_EXPANDED = 200;
	static constexpr int ENTRY_HEIGHT_EXPANDED = 200;

    QPoint mouseClickPosition;
    QPoint mousePosition;
    bool dragging = false;
    bool allowSpriteReordering = true;

    // Index of the sprite that/when the user is dragging
    int selectedIndexDragging = -1;

    // Index of the sprite the user has selected by clicking on it
	int selectedIndex = -1;

    int selectedRegionIndex = 0;

    std::vector<PK2::SpriteBase>* sprites = nullptr;

    ImageCache* imageCache = nullptr;
    const Region* currentRegion = nullptr;

    // TODO Delete this!
    const std::vector<QPixmap*>* spritePixmaps = nullptr;

	QStaticText stPlayer;
	int playerTextWidth = 0; // Using this because stPlayer.textWidth returns -1 for whatever reason.

	QFont fontCurrent;
	QFont fontBold;

    // TODO Delete this
    const std::vector<Region>* regionList = nullptr;

    // TODO Load this from a profiles file?
	QString typesAsString[6] = {
		"None",
		"Character",
		"Bonus Item",
		"Ammo",
		"Teleport",
		"Background"		
	};
};

