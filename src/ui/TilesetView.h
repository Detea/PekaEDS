#pragma once
#include <qwidget.h>
#include <qrect.h>

#include "TilesetRenderer.h"
#include "PekaEDSWindow.h"

#include "ImageCache.h"

#include <vector>

class PekaEDSWindow;

class TilesetView : public QWidget {
	Q_OBJECT

public:
	TilesetView(const PekaEDSWindow* mainWindow);

    // TODO Ideally this would be const
    void setImageCache(ImageCache& imageCache);

protected:
	// TODO Create MouseHandlers for all components? TilesetMouseHandler, MapMouseHandler
	void mousePressEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;

	void paintEvent(QPaintEvent* event) override;

signals:
    void tileSelectionChanged(const std::vector<int>& selection, int selectionWidth, int selectionHeight);

public slots:
    void regionsLoaded(const std::vector<Region>* newRegionList);
    void selectedRegionIndexChanged(int newSelectedRegion);

    void selectedLayerChanged(int newLayer);

private:
    void drawSelection(QPainter& painter, int x, int y, int width, int height);

private:
	int tilesetWidth = 0;
	int tilesetHeight = 0;

    // TODO Ideally this would be const
    ImageCache* imageCache = nullptr;

    const std::vector<Region>* regionList = nullptr;
    int selectedRegion = 0;
    QString currentTileset;

    int selectedLayer = 0;

	const PekaEDSWindow* mainWindow = nullptr;

    std::vector<int> selectedTiles;

    bool dragging = false;

    QString previewIdString;
    int previewX = -1;
    int previewY = -1;

    QPoint clickPosition;
	QRect selectionRect;
};
