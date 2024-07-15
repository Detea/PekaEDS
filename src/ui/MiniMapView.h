#pragma once
#include <qwidget.h>
#include <qrect.h>
#include <qsize.h>
#include <qimage.h>
#include <qpixmap.h>
#include <qtypes.h>
#include <qlist.h>
#include <memory>

#include "Region.h"

#include "PekaEDSWindow.h"

#include "ImageCache.h"
#include "Layers.h"

class PekaEDSWindow;

/*
	This mini map only works for the standard 1.3 maps and 8 bit bitmaps.
	Because it relies on certain tile id's being the same and the tileset having 150 tiles.

    IMPORTANT: All functions operate on the currently selected region!
*/
class MiniMapView : public QWidget {
	Q_OBJECT

public: 
	MiniMapView(const PekaEDSWindow* mainWindow);

    void setImageCache(ImageCache& imageCache);

public slots:
	void setViewportX(int newX);
	void setViewportY(int newY);

	void setViewportSize(const QSize& size);

    // Stores a pointer to the main region list that contains all loaded and future added regions
    void regionsLoaded(const std::vector<Region>* newRegionList);
    void selectedRegionChanged(int selectedRegionIndex);

    // Gets called when a map has been loaded
    void mapRegionsLoaded(const std::vector<Region>* loadedList);

    void foregroundTilesetChanged();
    void backgroundTilesetChanged();
    void backgroundImageChanged();

    void regionAdded(const Region& region);
    // TODO Implement
    //void selectedRegionDeleted(int index);
    //void selectedRegionResized(int index); // TODO [CLEAN UP] I don't think the index is necessary...

    // This method gets called every time the user places a tile in a region and updates only the affected parts
    void updateMap(int x, int y, int selectionWidth, int selectionHeight, Layer layer);

signals:
	void viewportXChanged(int newX);
	void viewportYChanged(int newY);

	void viewportSizeChanged(const QSize& newSize);

protected:
	void mousePressEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;

	void paintEvent(QPaintEvent* event) override;

    // void resizeEvent(QResizeEvent* e) override;

private:
    /*
     * Creates the mini map image of the added region
     * Only gets called once when:
     *  - A region has been added
     *  - The foreground tileset of a region has changed
     *  - The background tileset of a region has changed
     *
     * Calls update() if redraw == true
     */
    void drawMap(bool redraw = true);
    void cacheColors();

    void moveViewport(int newX, int newY);

private:
    static inline constexpr QColor BG_COLOR = QColor(31, 33, 54);
    const PekaEDSWindow* mainWindow = nullptr;

    // TODO [CLEAN UP] Ideally this would be const
    ImageCache* imageCache = nullptr;

    const std::vector<Region>* regionList = nullptr;
    int selectedRegion = 0;

    std::vector<std::unique_ptr<QImage>> regionMaps;
    QList<QRgb> foregroundTileColors;
    QList<QRgb> backgroundTileColors;
    QList<QRgb> backgroundColorPalette;

	QRect viewport;
    int mapOffsetX = 0;
    int mapOffsetY = 0;
};

