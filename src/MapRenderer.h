#pragma once
#include <qpainter.h>
#include <qsize.h>
#include <qwidget.h>
#include <qpixmap.h>

#include <vector>

#include "PK2MapData.h"

class MapRenderer : public QObject {
	Q_OBJECT

public:
	MapRenderer();

	void setViewSize(const QSize& newSize);

	void draw(QPainter& painter);

	void setShowSpriteBase(bool show);

public slots:
	void tilesetChanged(const QImage* newTileset);
	void backgroundChanged(const QImage* newBackground);

	void setViewX(int newX);
	void setViewY(int newY);

	void mapDataChanged(const PK2::MapData& newMapData);

private:
	// TODO Handle in/active layer drawing
	void drawTileLayer(QPainter& painter, const std::vector<int>& layer, int startX, int startY, int endX, int endY);
	void drawBackgroundSprites(QPainter& painter, const std::vector<int>& layer, int startX, int startY, int endX, int endY);
	void drawForegroundSprites(QPainter& painter, const std::vector<int>& layer, int startX, int startY, int endX, int endY);

	void drawSpriteHighlights(QPainter& painter, const std::vector<int>& layer, int startX, int startY, int endX, int endY);

private:
	int activeLayer = 0;

	QRectF backgroundRect;
	QPixmap backgroundImagePixmap;

	// TODO make these customizable?
	int mapWidth = 256;
	int mapHeight = 224;

	int viewportX = 0;
	int viewportY = 0;

	int viewportWidth = 0;
	int viewportHeight = 0;

	const QImage* tilesetImage = nullptr;
	const QImage* backgroundImage = nullptr;

	bool highlightSpritePositions = true;

	const PK2::MapData* mapData = nullptr;
};

