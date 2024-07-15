#include "MiniMapView.h"

#include <qpainter.h>

MiniMapView::MiniMapView(const PekaEDSWindow* mainWindow) : mainWindow(mainWindow) {
	setMouseTracking(true);

    setMinimumSize(256, 224);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

void MiniMapView::paintEvent(QPaintEvent* e) {
    if (!regionMaps.empty()) {
        QPainter painter(this);

        painter.drawImage(mapOffsetX, mapOffsetY, *regionMaps[selectedRegion]);

        painter.setPen(Qt::white);
        painter.drawRect(viewport.x() + mapOffsetX, viewport.y() + mapOffsetY, viewport.width(), viewport.height());
    }
}

void MiniMapView::drawMap(bool redraw) {
    if (regionList) {
        const Region& region = regionList->at(selectedRegion);

        for (int x = 0; x < region.width; ++x) {
            for (int y = 0; y < region.height; ++y) {
                int foregroundTile = region.foregroundLayer[x + region.width * y];
                int backgroundTile = region.backgroundLayer[x + region.width * y];

                if (foregroundTile != 255) {
                    regionMaps[selectedRegion]->setPixel(x, y, backgroundColorPalette[foregroundTileColors[foregroundTile]]);
                } else if (backgroundTile != 255) {
                    regionMaps[selectedRegion]->setPixel(x, y, backgroundColorPalette[backgroundTileColors[backgroundTile]]);
                }
            }
        }

        if (redraw) update();
    }
}

void MiniMapView::regionAdded(const Region& region) {
    regionMaps.push_back(std::make_unique<QImage>(region.width, region.height, QImage::Format_RGB32));

    regionMaps.back()->fill(BG_COLOR);
    cacheColors();

    drawMap(false);
}

void MiniMapView::mapRegionsLoaded(const std::vector<Region>* loadedList) {
    regionList = loadedList;

    regionMaps.clear();

    for (const Region& region : *loadedList) {
        regionAdded(region);
    }
}

void MiniMapView::foregroundTilesetChanged() {
    const Region& region = regionList->at(selectedRegion);
    foregroundTileColors = imageCache->getTilesetMiniMapColors(region.tilesetImageFile, region.backgroundImageFile);

    drawMap();
}

void MiniMapView::backgroundTilesetChanged() {
    const Region& region = regionList->at(selectedRegion);
    QString tilesetFile = region.tilesetBackgroundImageFile.isEmpty() ? region.tilesetImageFile : region.tilesetBackgroundImageFile;
    backgroundTileColors = imageCache->getTilesetMiniMapColors(tilesetFile, region.backgroundImageFile);

    /*
     * Note: It might be a bit faster to update each layer separately,
     * but it's probably going to be fast enough to redraw the whole map.
     *
     * The user won't change the tilesets that often and it will take them time to select a file, etc.
     */
    drawMap();
}

void MiniMapView::backgroundImageChanged() {
    cacheColors();

    drawMap();
}

void MiniMapView::updateMap(int x, int y, int selectionWidth, int selectionHeight, Layer layer) {
    const Region& region = regionList->at(selectedRegion);

    Q_ASSERT(!backgroundColorPalette.empty());
    Q_ASSERT(!foregroundTileColors.empty());
    Q_ASSERT(!backgroundTileColors.empty());

    for (int sy = y; sy < y + selectionHeight; ++sy) {
        for (int sx = x; sx < x + selectionWidth; ++sx) {
            if (layer == Layer::BACKGROUND) {
                int foregroundTile = region.foregroundLayer[sx + sy * region.width];
                int backgroundTile = region.backgroundLayer[sx + sy * region.width];

                if (foregroundTile == 255) {
                    if (backgroundTile != 255) {
                        regionMaps[selectedRegion]->setPixel(sx, sy, backgroundColorPalette[backgroundTileColors[backgroundTile]]);
                    } else {
                        // else remove this pixel
                        regionMaps[selectedRegion]->setPixel(sx, sy, BG_COLOR.rgb());
                    }
                }
            } else {
                int foregroundTile = region.foregroundLayer[sx + sy * region.width];

                if (foregroundTile != 255) {
                    regionMaps[selectedRegion]->setPixel(sx, sy, backgroundColorPalette[foregroundTileColors[foregroundTile]]);
                } else {
                    regionMaps[selectedRegion]->setPixel(sx, sy, BG_COLOR.rgb());
                }
            }
        }
    }

    update();
}

void MiniMapView::selectedRegionChanged(int selectedRegionIndex) {
    selectedRegion = selectedRegionIndex;

    viewport.moveTo(0, 0);

    emit viewportXChanged(0);
    emit viewportYChanged(0);

    setFixedSize(regionMaps[selectedRegion]->width(), regionMaps[selectedRegion]->height());

    mapOffsetX = (width() / 2) - (regionMaps[selectedRegion]->width() / 2);
    mapOffsetY = (height() / 2) - (regionMaps[selectedRegion]->height() / 2);

    cacheColors();

    update();
}

void MiniMapView::cacheColors() {
    const Region& region = regionList->at(selectedRegion);
    foregroundTileColors = imageCache->getTilesetMiniMapColors(region.tilesetImageFile, region.backgroundImageFile);

    QString tilesetFile = region.tilesetBackgroundImageFile.isEmpty() ? region.tilesetImageFile : region.tilesetBackgroundImageFile;
    backgroundTileColors = imageCache->getTilesetMiniMapColors(tilesetFile, region.backgroundImageFile);

    backgroundColorPalette = imageCache->getBackgroundPalette(region.backgroundImageFile);
}

void MiniMapView::setImageCache(ImageCache& imageCache) {
    this->imageCache = &imageCache;
}

void MiniMapView::regionsLoaded(const std::vector<Region>* newRegionList) {
    regionList = newRegionList;
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

	viewport.setSize(adjustedSize);
}

void MiniMapView::mousePressEvent(QMouseEvent* e) {
    if (e->buttons() & Qt::LeftButton) {
        // if (e->pos().x() >= mapOffsetX &&
        //         e->pos().x() + viewport.width() <= mapOffsetX + regionList->at(selectedRegion).width &&
        //         e->pos().y() >= mapOffsetY &&
        //         e->pos().y() + viewport.height() <= mapOffsetY + regionList->at(selectedRegion).height) {

        // }

        moveViewport(e->pos().x() - mapOffsetX, e->pos().y() - mapOffsetY);
    }
}

void MiniMapView::mouseMoveEvent(QMouseEvent* e) {
    if (e->buttons() & Qt::LeftButton) {
        moveViewport(e->pos().x() - mapOffsetX, e->pos().y() - mapOffsetY);
    }
}

// void MiniMapView::resizeEvent(QResizeEvent* e) {
//     mapOffsetX = (width() / 2) - (regionMaps[selectedRegion]->width() / 2);
//     mapOffsetY = (height() / 2) - (regionMaps[selectedRegion]->height() / 2);
// }

void MiniMapView::moveViewport(int newX, int newY) {
    // TODO [IMPLEMENT] Remove offset when emitting signal, keep view within map coordinates + mapOffsets
    QPoint newPos;

    int halfViewportWidth = viewport.width() / 2;
    int halfViewportHeight = viewport.height() / 2;

    newPos.setX(newX - halfViewportWidth);
    newPos.setY(newY - halfViewportHeight);

    int mapWidth = regionList->at(selectedRegion).width;
    int mapHeight = regionList->at(selectedRegion).height;

    if (newPos.x() < 0) newPos.setX(0);
    if (newPos.x() + viewport.width() >= mapWidth) newPos.setX(mapWidth - viewport.width() - 1);	// Subtracting 1 so that the right and bottom sides of the selection rectangle still appear

    if (newPos.y() < 0) newPos.setY(0);
    if (newPos.y() + viewport.height() >= mapHeight) newPos.setY(mapHeight - viewport.height() - 1);

    // These values get multiplied by 32 because the MiniMapView::setViewportX/Y slots also get used by MapView when its viewport position changes. This is used to keep the scroll bars and minimap in sync
    emit viewportXChanged(newPos.x() * 32);
    emit viewportYChanged(newPos.y() * 32);
}
