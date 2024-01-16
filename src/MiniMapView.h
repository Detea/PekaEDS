#pragma once
#include <qwidget.h>
#include <qrect.h>
#include <qsize.h>
#include <qimage.h>
#include <qpixmap.h>
#include <qtypes.h>
#include <qlist.h>

#include <vector>

#include "PekaEDSWindow.h"

#include "MiniMapInputHandler.h"
#include "PK2MapData.h"

class PekaEDSWindow;

/*
	This mini map only works for the standard 1.3 maps and 8 bit bitmaps.
	Because it relies on certain tile id's being the same and the tileset having 150 tiles.
*/
class MiniMapView : public QWidget {
	Q_OBJECT

public: 
	MiniMapView(const PekaEDSWindow* mainWindow);

public slots:
	void setViewportX(int newX);
	void setViewportY(int newY);

	void setViewportSize(const QSize& size);

// TODO Connect these signals in PekaEDSWindow with MapView, loose coupling!
signals:
	void viewportXChanged(int newX);
	void viewportYChanged(int newY);

	void viewportSizeChanged(const QSize& newSize);

protected:
	void mousePressEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;

	void paintEvent(QPaintEvent* event) override;

private slots:
	void mapDataChanged(const PK2::MapData& newMapData);

	void tilestImageChanged(const QImage* newTileset);
	void backgroundImageChanged(const QImage* newBackground);
	// TODO Not sure that tilesetImageChanged gets called when the background changes 

private:
	void connectSlots();

	void setTileColors(const QImage* tilesetImage, int tilesetWidth);

	void drawMap();

private:
	static constexpr unsigned char TILE_SWITCH2 = 146;
	static constexpr unsigned char TILE_SWITCH3 = 147;
	static constexpr unsigned char TILE_SWITCH1 = 145;
	static constexpr unsigned char TILE_START = 148;
	static constexpr unsigned char TILE_END = 149;

	int mapWidth = 256;
	int mapHeight = 224;

	const PekaEDSWindow* mainWindow = nullptr;

	const std::vector<std::vector<int>>* tileLayers = nullptr;
	const QImage* tilesetImage = nullptr;

	QImage mapImage;	
	
	QList<QRgb> colorPalette;

	int tileColors[150];

	QRect viewport;

	MiniMapInputHandler inputHandler;
};

