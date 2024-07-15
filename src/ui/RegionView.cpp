#include "RegionView.h"
#include "PK2SpriteTypes.h"
#include "Tool.h"

#include <qdebug.h>

#include <qpainter.h>
#include <QResizeEvent>

RegionView::RegionView(const PekaEDSWindow* mainWindow) : mainWindow(mainWindow) {
	setupUI();
}

void RegionView::setupUI() {
    setFocusPolicy(Qt::StrongFocus);

    setMouseTracking(true);
}

void RegionView::paintEvent(QPaintEvent* event) {
	QPainter painter(this);

    if (mapMetadata && regionList) {
        const Region& region = regionList->at(selectedRegion);

        const QPixmap* bgPixmap = imageCache->getBackground(region.backgroundImageFile);
        if (!region.missingBackground) {
            painter.drawTiledPixmap(backgroundRect, *bgPixmap);
        } else {
            // TODO [FIX] Paint background color? Or set it via QWidget's CSS with custom themes?
        }

        painter.scale(scale, scale);

		int viewWidthClamped = viewXMap + viewWidthMap + 1;
		int viewHeightClamped = viewYMap + viewHeightMap + 1;

        // TODO [CHECK] is - 1 correct here?
        if (viewWidthClamped >= region.width) viewWidthClamped = region.width - 1;
        if (viewHeightClamped >= region.height) viewHeightClamped = region.height - 1;

        if (activeLayer == Layer::FOREGROUND) {
			painter.setOpacity(0.5);
		} else {
			painter.setOpacity(1);
		}
        drawTileLayer(painter, Layer::BACKGROUND, viewXMap, viewYMap, viewWidthClamped, viewHeightClamped);

		painter.setOpacity(1);
        if (showBackgroundSprites) drawBackgroundSprites(painter, region.spriteLayer, viewXMap, viewYMap, viewWidthClamped, viewHeightClamped);
        if (showForegroundSprites) drawForegroundSprites(painter, region.spriteLayer, viewXMap, viewYMap, viewWidthClamped, viewHeightClamped);

        if (activeLayer == Layer::BACKGROUND) {
			painter.setOpacity(0.5);
		} else {
			painter.setOpacity(1);
		}
        drawTileLayer(painter, Layer::FOREGROUND, viewXMap, viewYMap, viewWidthClamped, viewHeightClamped);

        if (highlightSprites || showSpriteSize) {
            drawSpriteHighlights(painter, region.spriteLayer, viewXMap, viewYMap, viewWidthClamped, viewHeightClamped);
		}

        // Draw tools
        painter.setOpacity(1);
        if (selectedTool && selectedSecondaryTool) {
            const QString& tilesetFile = !region.tilesetBackgroundImageFile.isEmpty() &&
                    activeLayer == Layer::BACKGROUND ?
                        region.tilesetBackgroundImageFile :
                        region.tilesetImageFile;

            toolToDraw->draw(painter, tilesetFile, region.backgroundImageFile);
        }
	}
}

void RegionView::drawTileLayer(QPainter& painter, Layer layerToDraw, int startX, int startY, int endX, int endY) {
    const Region& region = regionList->at(selectedRegion);

    const std::vector<int>& layer = layerToDraw == Layer::FOREGROUND ? region.foregroundLayer : region.backgroundLayer;

    // TODO [CLEAN UP] Might be able to cache this when the layer changes? Better than doing it every repaint
    const QString& tilesetFile = !region.tilesetBackgroundImageFile.isEmpty() &&
            layerToDraw == Layer::BACKGROUND ?
                region.tilesetBackgroundImageFile :
                region.tilesetImageFile;

    for (int y = startY; y < endY; ++y) {
		for (int x = startX; x < endX; ++x) {
            int tile = layer[x + y * regionList->at(selectedRegion).width];

            // TODO [CLEAN UP] Don't hardcode the empty tile id?
			if (tile != 255) {
				// Position of the tile in the tileset image
				int tx = (tile % 10) * 32;
				int ty = (tile / 10) * 32;

				int drawX = (x - startX) * 32;
				int drawY = (y - startY) * 32;

                painter.drawPixmap(drawX, drawY, imageCache->getTileset(tilesetFile, region.backgroundImageFile), tx, ty, 32, 32);
			}
		}
	}
}

void RegionView::drawBackgroundSprites(QPainter& painter, const std::vector<int>& layer, int startX, int startY, int endX, int endY) {
    // TODO [FIX] Render 6 "tiles" of sprites outside of the view so bigger sprites don't disappear when their base position isn't in view
    const Region& region = regionList->at(selectedRegion);

    /*
    if (startX - toleranceX >= 0) startX -= toleranceX;
    if (startY - toleranceY >= 0) startY -= toleranceY;

    if (endX + toleranceX < region.width) endX += toleranceX;
    if (endY + toleranceY < region.height) endY += toleranceY;
    */

	for (int y = startY; y < endY; ++y) {
		for (int x = startX; x < endX; ++x) {
            int spriteIndex = layer[x + y * region.width];

			// TODO Don't hardcode the empty tile id
			if (spriteIndex != 255) {
                if (spriteIndex >= 0 && spriteIndex < mapMetadata->spriteObjects.size()) {
                    const PK2::SpriteBase& sprite = mapMetadata->spriteObjects[spriteIndex];

                    if (sprite.type == PK2::SpriteTypes::BACKGROUND) {
                        int sprXAdjusted = ((x - startX) * 32) - (sprite.frameWidth / 2) + 16;
                        int sprYAdjusted = ((y - startY) * 32) - (sprite.frameHeight - 32);

                        painter.drawPixmap(sprXAdjusted,
                                           sprYAdjusted,

                                           imageCache->getSpriteImage(sprite, region.backgroundImageFile),

                                           sprite.frameX,
                                           sprite.frameY,

                                           sprite.frameWidth,
                                           sprite.frameHeight);
					}
				}
			}
		}
	}
}

void RegionView::drawForegroundSprites(QPainter& painter, const std::vector<int>& layer, int startX, int startY, int endX, int endY) {
    const Region& region = regionList->at(selectedRegion);

    /*
    if (startX - toleranceX >= 0) startX -= toleranceX;
    if (startY - toleranceY >= 0) startY -= toleranceY;

    if (endX + toleranceX < region.width) endX += toleranceX;
    if (endY + toleranceY < region.height) endY += toleranceY;
    */

    for (int y = startY; y < endY; ++y) {
		for (int x = startX; x < endX; ++x) {
            int spriteIndex = layer[x + y * region.width];

			// TODO Don't hardcode the empty tile id
			if (spriteIndex != 255) {
                if (spriteIndex >= 0 && spriteIndex < mapMetadata->spriteObjects.size()) {
                    const PK2::SpriteBase& sprite = mapMetadata->spriteObjects[spriteIndex];

                    // TODO Why does this not check for FOREGROUND?
                    if (sprite.type != PK2::SpriteTypes::BACKGROUND) {
                        int sprXAdjusted = ((x - startX) * 32) - (sprite.frameWidth / 2) + 16;
                        int sprYAdjusted = ((y - startY) * 32) - (sprite.frameHeight - 32);

                        painter.drawPixmap(sprXAdjusted,
                                           sprYAdjusted,

                                           imageCache->getSpriteImage(sprite, region.backgroundImageFile),

                                           sprite.frameX,
                                           sprite.frameY,

                                           sprite.frameWidth,
                                           sprite.frameHeight);
					}
				}
			}
		}
	}
}

void RegionView::drawSpriteHighlights(QPainter& painter, const std::vector<int>& layer, int startX, int startY, int endX, int endY) {
    const Region& region = regionList->at(selectedRegion);

    for (int y = startY; y < endY; ++y) {
		for (int x = startX; x < endX; ++x) {
            int spriteIndex = layer[x + y * region.width];

			// TODO Don't hardcode the empty tile id
			if (spriteIndex != 255) {
                if (spriteIndex >= 0 && spriteIndex < mapMetadata->spriteObjects.size()) {
                    const PK2::SpriteBase* sprite = &mapMetadata->spriteObjects[spriteIndex];
					if (sprite->frameWidth > 32 || sprite->frameHeight > 32) {
						int sprXAdjusted = ((x - startX) * 32) - (sprite->frameWidth / 2) + 16;
						int sprYAdjusted = ((y - startY) * 32) - (sprite->frameHeight - 32);
						
                        if (showSpriteSize) {
                            painter.setPen(Qt::lightGray);
                            painter.drawRect(sprXAdjusted, sprYAdjusted, sprite->frameWidth, sprite->frameHeight);
                        }
					}

                    if (highlightSprites) {
                        painter.setPen(Qt::black);
                        painter.drawRect((x - startX) * 32, (y - startY) * 32, 32, 32);

                        painter.setPen(Qt::white);
                        painter.drawRect((x - startX) * 32 + 1, (y - startY) * 32 + 1, 30, 30);
                    }
				}
			}
		}
	}
}

void RegionView::resizeEvent(QResizeEvent* e) {
    viewWidth = e->size().width() * (1 + scale);
    viewHeight = e->size().height() * (1 + scale);

    viewWidthMap = viewWidth / 32;
	viewHeightMap = viewHeight / 32;

	backgroundRect.setWidth(viewWidth);
	backgroundRect.setHeight(viewHeight);

	emit viewportSizeChanged(e->size());
}

/*
	Mouse events
*/
void RegionView::mousePressEvent(QMouseEvent* event) {
    if (regionList) {
        QPoint adjustedPosition = event->pos();
        adjustedPosition.setX(adjustedPosition.x() * scale);
        adjustedPosition.setY(adjustedPosition.y() * scale);

        Tool::setMousePosition(adjustedPosition);
        Tool::setDragging(true);

        if (selectedTool && event->button() == Qt::LeftButton) {
            selectedTool->mousePressed();

            toolToDraw = selectedTool;

            update();
        } else if (selectedSecondaryTool && event->button() == Qt::RightButton) {
            selectedSecondaryTool->mousePressed();

            toolToDraw = selectedSecondaryTool;

            update();
        }
    }
}

void RegionView::mouseMoveEvent(QMouseEvent* event) {
    if (regionList) {
        QPoint adjustedPosition = event->pos();
        adjustedPosition.setX(adjustedPosition.x() * scale);
        adjustedPosition.setY(adjustedPosition.y() * scale);

        Tool::setMousePosition(adjustedPosition);

        if (selectedTool && (event->buttons() == Qt::LeftButton)) {
            selectedTool->mouseMoved();

            toolToDraw = selectedTool;
        }

        if (selectedSecondaryTool && (event->buttons() == Qt::RightButton)) {
            selectedSecondaryTool->mouseMoved();

            toolToDraw = selectedSecondaryTool;
        }

        update();
    }
}

void RegionView::mouseReleaseEvent(QMouseEvent* event) {
    if (regionList) {
        QPoint adjustedPosition = event->pos();
        adjustedPosition.setX(adjustedPosition.x() * scale);
        adjustedPosition.setY(adjustedPosition.y() * scale);

        Tool::setMousePosition(adjustedPosition);
        Tool::setDragging(false);

        if (selectedTool && event->button() == Qt::LeftButton) {
            selectedTool->mouseReleased();

            update();
        } else if (selectedSecondaryTool && event->button() == Qt::RightButton) {
            selectedSecondaryTool->mouseReleased();

            update();
        }

        toolToDraw = selectedTool;
    }
}

void RegionView::wheelEvent(QWheelEvent* e) {
    if (e->angleDelta().y() < 0) {
        // Zoom out
        scale -= 0.1f;

        viewWidthMap = (viewWidth * (1 + scale)) / 32;
        viewHeightMap = (viewHeight * (1 + scale)) / 32;
    } else {
        // Zoom in
        scale += 0.1f;

        viewWidthMap = (viewWidth / scale) / 32;
        viewHeightMap = (viewHeight / scale) / 32;
    }

    update();
}

void RegionView::keyPressEvent(QKeyEvent* e) {
    switch (e->key()) {
    case Qt::Key_Left:
        viewX -= 32;
        break;

    case Qt::Key_Right:
        viewX += 32;
        break;

    case Qt::Key_Up:
        viewY -= 32;
        break;

    case Qt::Key_Down:
        viewY += 32;
        break;
    }

    const Region& region = regionList->at(selectedRegion);

    if (viewX < 0) viewX = 0;
    if (viewX + viewWidth > region.width * 32) viewX = (region.width * 32) - viewWidth;

    if (viewY < 0) viewY = 0;
    if (viewY + viewHeight > region.height * 32) viewY = (region.height * 32) - viewHeight;

    viewXMap = viewX / 32;
    viewYMap = viewY / 32;

    emit viewportXChanged(viewX);
    emit viewportYChanged(viewY);

    update();
}

void RegionView::setImageCache(ImageCache& imageCache) {
    this->imageCache = &imageCache;
}

/*
		Slots
*/

void RegionView::mapChanged() {
    update();
}

void RegionView::setViewportX(int newX) {
	int oldX = viewX;
    viewX = newX;

	viewXMap = viewX / 32;

    if (oldX != newX) {
        Tool::setViewX(viewX);

        emit viewportXChanged(newX);
    }

	update();
}

void RegionView::setViewportY(int newY) {
	int oldY = viewY;
    viewY = newY;

	viewYMap = viewY / 32;

    if (oldY != newY) {
        Tool::setViewY(viewY);

        emit viewportYChanged(newY);
    }

	update();
}

void RegionView::regionsLoaded(const std::vector<Region>* newList) {
    Q_ASSERT(newList != nullptr);

    regionList = newList;
}

void RegionView::selectedRegionChanged(int newRegionIndex) {
    selectedRegion = newRegionIndex;

    update();
}

void RegionView::tilesetChanged(const QString& newTileset) {
	update();
}

void RegionView::backgroundChanged(const QImage* newBackground) {
	backgroundImage = newBackground;

	backgroundPixmap.convertFromImage(*newBackground);
	backgroundRect = backgroundPixmap.rect();

	update();
}

void RegionView::mapMetadataChanged(const PK2::MapBaseMetadata *newData) {
    mapMetadata = newData;

	update();
}

void RegionView::setHighlightSprites(bool highlight) {
	highlightSprites = highlight;

	update();
}

void RegionView::setShowSpriteSize(bool showSize) {
    showSpriteSize = showSize;

    update();
}

void RegionView::setShowForegroundSprites(bool show) {
	showForegroundSprites = show;

	update();
}

void RegionView::setShowBackgroundSprites(bool show) {
	showBackgroundSprites = show;

	update();
}

void RegionView::setSelectedLayer(int layer) {
	activeLayer = layer;

	update();
}

void RegionView::selectedToolChanged(Tool* newSelectedTool) {
    selectedTool = newSelectedTool;

    toolToDraw = selectedTool;
}

void RegionView::selectedSecondaryToolChanged(Tool* newSelectedTool) {
    selectedSecondaryTool = newSelectedTool;
}
