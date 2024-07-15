#include "TilesetView.h"
#include <qpainter.h>
#include <qevent.h>
#include <QPainterPath>

#include "MapUtil.h"

#include "Layers.h"

TilesetView::TilesetView(const PekaEDSWindow* parentWindow) : QWidget(parentWindow->parentWidget()), mainWindow(parentWindow) {
	setMinimumSize(320, 480);

    setMouseTracking(true);

    setFont(QFont(font().family(), 11));
}

void TilesetView::paintEvent(QPaintEvent* event) {
	QPainter painter(this);

    if (!currentTileset.isEmpty()) {
        painter.fillRect(0, 0, width(), height(), Qt::darkGray);
        painter.drawPixmap(0, 0, imageCache->getTileset(currentTileset, regionList->at(selectedRegion).backgroundImageFile));

        drawSelection(painter, selectionRect.x(), selectionRect.y(), selectionRect.width(), selectionRect.height());

        if (!dragging && selectedTiles.size() <= 1) {
            // TODO Center this
            // TODO Give the text an outline?
            // int textX = previewX + (16 - (painter.fontMetrics().boundingRect(previewIdString).width() / 2));
            int textX = previewX + (16 - (painter.fontMetrics().horizontalAdvance(previewIdString) / 2));
            int textY = previewY + (painter.fontMetrics().height() * 1.5f);

            drawSelection(painter, previewX, previewY, 32, 32);

            painter.setPen(Qt::white);
            painter.drawText(textX, textY, previewIdString);
        }
    }
}

void TilesetView::drawSelection(QPainter& painter, int x, int y, int width, int height) {
    // Outer rect
    painter.setPen(Qt::black);
    painter.drawRect(x, y, width - 1, height - 1);

    // Inner rect
    painter.drawRect(x + 2, y + 2, width - 5, height - 5);

    // Middle rect
    painter.setPen(Qt::white);
    painter.drawRect(x + 1, y + 1, width - 3, height - 3);
}

void TilesetView::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        int mx = event->pos().x();
        int my = event->pos().y();

        if (mx >= 0 && mx <= width() &&
            my >= 0 && my <= height()) {

            dragging = true;

            selectionRect.setX(MapUtil::alignToGrid(mx));
            selectionRect.setY(MapUtil::alignToGrid(my));

            selectionRect.setWidth(MapUtil::TILE_SIZE);
            selectionRect.setHeight(MapUtil::TILE_SIZE);

            clickPosition.setX(selectionRect.x());
            clickPosition.setY(selectionRect.y());
        }

        repaint();
    }
}

void TilesetView::mouseMoveEvent(QMouseEvent* event) {
    int mouseX = MapUtil::alignToGrid(event->pos().x());
    int mouseY = MapUtil::alignToGrid(event->pos().y());

    if (dragging) {
        if (mouseX < 0) mouseX = 0;
        if (mouseY < 0) mouseY = 0;

        if (mouseX >= width()) mouseX = width() - 32;
        if (mouseY >= height()) mouseY = height() - 32;

        MapUtil::calculateSelectionRect(selectionRect, clickPosition.x(), clickPosition.y(), mouseX, mouseY);

        update();
    } else {
        previewIdString = QString::number((mouseX / 32) + ((mouseY / 32) * 10) + 1);

        previewX = MapUtil::alignToGrid(mouseX);
        previewY = MapUtil::alignToGrid(mouseY);
    }

    update();
}

void TilesetView::mouseReleaseEvent(QMouseEvent* event) {
    dragging = false;

    int sx = MapUtil::positionToMap(selectionRect.x());
    int sy = MapUtil::positionToMap(selectionRect.y());
    int sw = MapUtil::positionToMap(selectionRect.width());
    int sh = MapUtil::positionToMap(selectionRect.height());

    selectedTiles.clear();
    selectedTiles.reserve(sw * sh);

    for (int y = sy; y < sy + sh; ++y) {
        for (int x = sx; x < sx + sw; ++x) {
            selectedTiles.push_back((y * tilesetWidth) + x);
        }
    }

    emit tileSelectionChanged(selectedTiles, sw, sh);
}

void TilesetView::selectedLayerChanged(int newLayer) {
    selectedLayer = newLayer;

    const Region& region = regionList->at(selectedRegion);
    if (selectedLayer == Layer::BACKGROUND && !region.tilesetBackgroundImageFile.isEmpty()) {
        currentTileset = region.tilesetBackgroundImageFile;
    } else {
        currentTileset = region.tilesetImageFile;
    }

    // Need to do this everytime the layer changes, because extended tilesets have a different size and if one is 320x480 and the other is extended, the size needs to be changed.
    const QPixmap& pixmap = imageCache->getTileset(currentTileset, region.backgroundImageFile);
    if (width() != pixmap.width() || height() != pixmap.height()) {
        setFixedSize(pixmap.width(), pixmap.height());
    }

    tilesetWidth = pixmap.width() / 32;

    // TODO Maybe store the last selected layer and only update if lastSelectedLayer == Layer::Background && !tilesetBgFile.isEmpty(), to avoid unecessary updates
    update();
}

void TilesetView::setImageCache(ImageCache& imageCache) {
    this->imageCache = &imageCache;
}

void TilesetView::regionsLoaded(const std::vector<Region>* newRegionList) {
    regionList = newRegionList;
}

void TilesetView::selectedRegionIndexChanged(int newSelectedRegion) {
    selectedRegion = newSelectedRegion;

    // TODO [CLEAN UP] same as in layerChanged, find a better way to do this
    const Region& region = regionList->at(selectedRegion);
    if (selectedLayer == Layer::BACKGROUND && !region.tilesetBackgroundImageFile.isEmpty()) {
        currentTileset = region.tilesetBackgroundImageFile;
    } else {
        currentTileset = region.tilesetImageFile;
    }

    update();
}
