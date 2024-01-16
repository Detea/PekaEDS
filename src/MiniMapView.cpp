#include "MiniMapView.h"

#include <qpainter.h>

MiniMapView::MiniMapView(const PekaEDSWindow* mainWindow) : mainWindow(mainWindow) {
	mapImage = QImage(mapWidth, mapHeight, QImage::Format_RGB32);
	mapImage.fill(QColor(31, 33, 54));

	inputHandler.setMapSize(mapWidth, mapHeight);

	setFixedSize(mapWidth, mapHeight);

	setMouseTracking(true);

	connectSlots();
}

void MiniMapView::connectSlots() {
	connect(mainWindow, &PekaEDSWindow::mapDataChanged, this, &MiniMapView::mapDataChanged);
	
	connect(mainWindow, &PekaEDSWindow::tilesetImageLoaded, this, &MiniMapView::tilestImageChanged);
	connect(mainWindow, &PekaEDSWindow::backgroundImageChanged, this, &MiniMapView::backgroundImageChanged);

	connect(&inputHandler, &MiniMapInputHandler::viewportXChanged, this, &MiniMapView::setViewportX);
	connect(&inputHandler, &MiniMapInputHandler::viewportYChanged, this, &MiniMapView::setViewportY);
}

void MiniMapView::paintEvent(QPaintEvent* event) {
	QPainter painter(this);

	painter.drawImage(0, 0, mapImage);
	
	painter.setPen(Qt::white);
	painter.drawRect(viewport.x(), viewport.y(), viewport.width(), viewport.height());
}

void MiniMapView::drawMap() {
	for (int layer = 0; layer < tileLayers->size(); ++layer) {
		for (int x = 0; x < mapWidth; ++x) {
			for (int y = 0; y < mapHeight; ++y) {
				int tile = tileLayers->at(layer)[x + mapWidth * y];

				if (tile != 255) {
					if (!colorPalette.empty()) {
						mapImage.setPixel(x, y, colorPalette[tileColors[tile]]);
					}
				}
			}
		}
	}
}

// This method calculates the average color of a tile in the tileset
void MiniMapView::setTileColors(const QImage* tilesetImage, int tilesetWidth) {
	for (int tile = 0; tile < 150; tile++) {
		// The games color palette technically has 7 primary colors, because the colors after those have special purposes
		unsigned long primaryColors[8];
		for (int i = 0; i < 8; i++) {
			primaryColors[i] = 0;
		}

		unsigned long primaryColor = 0;		// Primary color in the palette, will be the first entry for the specific color in the palette

		/*
			averageShadow contains the "shade" of the color, a value relative to primaryColor.

			Note: All these values are indices in the color palette.
			Let's say the tile has a primary color value of 64 and let's assume that that primary color is used as a red.
			The tile contains pixels that use the color index 73.
			The averageShadow is calculated by this: tilePixel % 32 (see below in the loop)
			73 % 32 = 9

			So the averageShadow for this pixel would be 9.
		*/
		unsigned long averageShadow = 0;

		// Gets increased if tilePixel (see below in loop) is one of the regular primary colors
		unsigned long tileNumber = 0;

		// Position of 'tile' in the tileset, used as offset for the next loop
		int tileX = tile % 10;
		int tileY = tile / 10;

		// Loop through all the pixels of the tile (whose index is stored in 'tile') and calculate the average color(shadow?)
		for (int xx = 0; xx < 32; xx++) {
			for (int yy = 0; yy < 32; yy++) {
				// This contains an index into the color palette, because we're working with a 256 color bitmap
				unsigned char tilePixel = (unsigned char) tilesetImage->pixelIndex((xx + (tileX * 32)), (yy + (tileY * 32)));

				// The colors in the palette at entries 224 and above have special purposes and are ignored
				if (tilePixel < 224) {
					primaryColor = tilePixel / 32;	// Get the primary color of the pixel
					primaryColors[primaryColor] = primaryColors[primaryColor] + 1;	// Color has been used, so increase it's count by one

					// Calculate the distance from the primaryColor value
					// Each color in the palette has 32 "shades"
					averageShadow = averageShadow + (tilePixel % 32);

					tileNumber++;
				}
			}
		}

		int highestPrimaryColor = 0;

		// Loop through the primary color and find the color that's used the most in the tile
		for (int i = 0; i < 8; i++) {
			unsigned long lastPrimaryColor = 0;

			if (primaryColors[i] > lastPrimaryColor) {
				highestPrimaryColor = i;

				lastPrimaryColor = primaryColors[i];
			}
		}

		// If color is valid (meaning it doesn't have a special purpose) its averageShadow gets calculated
		if (tileNumber > 0) {
			averageShadow = averageShadow / tileNumber;
		} else {
			averageShadow = 0;
		}

		if (tile < 90) {
			averageShadow += 3;
		}

		if (tile == TILE_SWITCH1 ||
			tile == TILE_SWITCH2 ||
			tile == TILE_SWITCH3 ||
			tile == TILE_START ||
			tile == TILE_END) {
			averageShadow += 12;
		}

		// Each color in the palette has 32 shades, so we keep clamp averageShadow to that if necessary
		if (averageShadow > 31) {
			averageShadow = 31;
		}

		tileColors[tile] = averageShadow + highestPrimaryColor * 32;
	}
}

void MiniMapView::mapDataChanged(const PK2::MapData& newMapData) {
	tileLayers = &newMapData.tileLayers;

	drawMap();

	update();
}

void MiniMapView::tilestImageChanged(const QImage* newTileset) {
	setTileColors(newTileset, newTileset->width());

	if (tileLayers) {
		drawMap();

		update();
	}
}

void MiniMapView::backgroundImageChanged(const QImage* newBackground) {
	colorPalette = newBackground->colorTable();

	update();
}

void MiniMapView::setViewportX(int newX) {
	int oldX = viewport.x();

	viewport.moveTo(newX / 32, viewport.y());

	if (oldX != newX) {
		emit viewportXChanged(newX);
	}

	update();
}

void MiniMapView::setViewportY(int newY) {
	int oldY = viewport.y();

	viewport.moveTo(viewport.x(), newY / 32);

	if (oldY != newY) {
		emit viewportYChanged(newY);
	}

	update();
}

void MiniMapView::setViewportSize(const QSize& size) {
	QSize adjustedSize(size.width() / 32, size.height() / 32);

	inputHandler.setViewportSize(adjustedSize);

	viewport.setSize(adjustedSize);
}

void MiniMapView::mousePressEvent(QMouseEvent* event) {
	inputHandler.mousePressEvent(event);
}

void MiniMapView::mouseMoveEvent(QMouseEvent* event) {
	inputHandler.mouseMoveEvent(event);
}