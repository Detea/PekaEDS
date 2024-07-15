#pragma once
#include <qwidget.h>

#include <qevent.h>

#include <qrect.h>

#include "PekaEDSWindow.h"
#include "PK2MapBaseMetadata.h"
#include "Region.h"
#include "Tool.h"
#include "ImageCache.h"
#include "Layers.h"

class PekaEDSWindow;

class RegionView : public QWidget {
	Q_OBJECT
public:
    RegionView(const PekaEDSWindow* mainWindow);	// TODO Parent???

    void setImageCache(ImageCache& imageCache);

public slots:
	void setViewportX(int newX);
	void setViewportY(int newY);

	void setSelectedLayer(int layer);

    void tilesetChanged(const QString& newTileset);
	void backgroundChanged(const QImage* newBackground);

    void mapMetadataChanged(const PK2::MapBaseMetadata* newData);
    //void mapDataChanged(const Region& newRegion);

    void setHighlightSprites(bool highlight);
    void setShowSpriteSize(bool showSize);
    void setShowForegroundSprites(bool show);
	void setShowBackgroundSprites(bool show);

    //void setRegion(const Region* region);

    void selectedToolChanged(Tool* newSelectedTool);
    void selectedSecondaryToolChanged(Tool* newSelectedTool);

    void regionsLoaded(const std::vector<Region>* newList);
    void selectedRegionChanged(int newRegionIndex);

    void mapChanged();

signals:
	void viewportXChanged(int newX);
	void viewportYChanged(int newY);
	void viewportSizeChanged(const QSize& newSize);

protected:
	void mousePressEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* e) override;

    void keyPressEvent(QKeyEvent* e) override;

	void resizeEvent(QResizeEvent* e) override;

	void paintEvent(QPaintEvent* event) override;

private:
	void setupUI();

    void drawTileLayer(QPainter& painter, Layer layer, int startX, int startY, int endX, int endY);
	void drawBackgroundSprites(QPainter& painter, const std::vector<int>& layer, int startX, int startY, int endX, int endY);
	void drawForegroundSprites(QPainter& painter, const std::vector<int>& layer, int startX, int startY, int endX, int endY);

	void drawSpriteHighlights(QPainter& painter, const std::vector<int>& layer, int startX, int startY, int endX, int endY);

private:
    inline static constexpr int toleranceX = 4;
    inline static constexpr int toleranceY = 4;

	const PekaEDSWindow* mainWindow = nullptr;

	bool highlightSprites = true;
    bool showSpriteSize = true;
	bool showForegroundSprites = true;
	bool showBackgroundSprites = true;

	const QImage* tilesetImage = nullptr;
	const QImage* backgroundImage = nullptr;
	QPixmap backgroundPixmap;

	/*
		backgroundRect contains the rectangle in which the background pixmap is going to be tiled in.
		Same size as the viewport, position will always be 0, 0.
	*/
	QRect backgroundRect;

	int mapWidth = 256;
	int mapHeight = 224;

	int viewX = 0;
	int viewY = 0;
	int viewWidth = 0;
	int viewHeight = 0;

	// These values are constrained within the mapWidth & mapHeight values. They are divided by TILE_SIZE (32).
	int viewXMap = 0;
	int viewYMap = 0;
	int viewWidthMap = 0;
	int viewHeightMap = 0;

    int activeLayer = Layer::BOTH;

    ImageCache* imageCache = nullptr;

    const std::vector<Region>* regionList = nullptr;
    int selectedRegion = 0;

    const PK2::MapBaseMetadata* mapMetadata = nullptr;
    const Region* regionToDisplay = nullptr;

    Tool* selectedTool = nullptr;
    Tool* selectedSecondaryTool = nullptr;

    Tool* toolToDraw = nullptr;

    qreal scale = 1.0;
};

