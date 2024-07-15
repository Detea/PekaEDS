#ifndef TOOL_H
#define TOOL_H

#include <QObject>
#include <QPainter>
#include <QMouseEvent>

#include "Region.h"
#include "ImageCache.h"
#include "PK2SpriteBase.h"

class Tool : public QObject {
    Q_OBJECT

public:
    enum MODE {
        TILE, SPRITE
    };

    Tool();

    virtual void draw(QPainter& painter, const QString& tileset, const QString& background) = 0;

    static void setMousePosition(const QPoint& position);
    static void setDragging(bool dragging);

    virtual void mousePressed() = 0;
    virtual void mouseReleased() = 0;
    virtual void mouseMoved() = 0;

    static void setMode(MODE newMode);

    static void setSpriteList(const std::vector<PK2::SpriteBase>* newSpriteList);
    static void setRegionList(const std::vector<Region>* newRegionList);
    static void setSelectedRegion(int newSelectedRegion);

    static void setImageCache(ImageCache& imageCache);

    static void setViewX(int newX);
    static void setViewY(int newY);

    static void setSelectedTiles(const std::vector<int>& newSelection, int selectionWidth, int selectionHeight);
    static void setSelectedSprite(int spriteNumber);
    static void setSelectedSprites(const std::vector<int>& newSelection, int sw, int sh);

signals:
    void tilePlaced(int x, int y, int selectionWidth, int selectionHeight, const std::vector<int>& selectedTiles);
    void spritePlaced(int x, int y, int selectionWidth, int selectionHeight, const std::vector<int>& selectedSprites);

protected:
    static MODE getMode();
    bool isDragging();

    const Region& getSelectedRegion();
    bool isRegionListValid();

    const std::vector<PK2::SpriteBase>* getSpriteList();

    static const QPoint& getMousePositionMap();   // See mousePositionMap
    static const QPoint& getMousePositionView();  // See mousePositionView

    static int getViewX();
    static int getViewY();

    inline static std::vector<int> selectedTiles;
    inline static std::vector<int> selectedSprites;
    inline static ImageCache* imageCache = nullptr;

    inline static int tileSelectionWidth = 0;
    inline static int tileSelectionHeight = 0;

    inline static int spriteSelectionWidth = 0;
    inline static int spriteSelectionHeight = 0;

private:
    inline static MODE mode = MODE::TILE;

    inline static const std::vector<Region>* regionList = nullptr;
    inline static int selectedRegion = 0;

    inline static const std::vector<PK2::SpriteBase>* spriteList = nullptr;

    // mousePositionMap is the position of the mouse on the map, which includes the view offsets.
    // Bounds: (0, 0, mapWidth, mapHeight)
    // Use this if you want to place tiles or sprites in the map.
    inline static QPoint mousePositionMap;

    // mousePositionView is the position of the mouse in the view.
    // Bounds: (0, 0, RegionWidth::width, RegionView::height.
    // Use this if you want to draw the tool in the view.
    inline static QPoint mousePositionView;

    // View offsets of RegionView
    inline static int viewX = 0;
    inline static int viewY = 0;

    inline static bool dragging = false;
};

#endif // TOOL_H
