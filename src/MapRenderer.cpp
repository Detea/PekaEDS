#include "MapRenderer.h"
#include "PK2SpriteTypes.h"

#include <qdebug.h>

MapRenderer::MapRenderer() {
}

// TODO Convert the tileset to QPixmap?
void MapRenderer::draw(QPainter& painter) {
	// TODO Draw background? Or use css for theme to do this?
	if (mapData) {
		painter.drawTiledPixmap(backgroundRect, backgroundImagePixmap);

		// TODO Make scrolling smooth

		int viewX = viewportX / 32;
		int viewY = viewportY / 32;

		int viewWidth = viewportWidth / 32;
		int viewHeight = viewportHeight / 32;

		int viewWidthClamped = viewX + viewWidth + 1;
		int viewHeightClamped = viewY + viewHeight + 1;

		if (viewWidthClamped >= mapWidth) viewWidthClamped = mapWidth - 1;
		if (viewHeightClamped >= mapHeight) viewHeightClamped = mapHeight - 1;

		drawTileLayer(painter, mapData->tileLayers[0], viewX, viewY, viewWidthClamped, viewHeightClamped);

		drawBackgroundSprites(painter, mapData->spriteLayers.at(0), viewX, viewY, viewWidthClamped, viewHeightClamped);
		drawForegroundSprites(painter, mapData->spriteLayers.at(0), viewX, viewY, viewWidthClamped, viewHeightClamped);

		drawTileLayer(painter, mapData->tileLayers[1], viewX, viewY, viewWidthClamped, viewHeightClamped);

		if (highlightSpritePositions) {
			drawSpriteHighlights(painter, mapData->spriteLayers[0], viewX, viewY, viewWidthClamped, viewHeightClamped);
		}
	}
}

void MapRenderer::tilesetChanged(const QImage* newTileset) {
	tilesetImage = newTileset;
}

void MapRenderer::backgroundChanged(const QImage* newBackground) {
	backgroundImage = newBackground;

	// fromImage?
	backgroundImagePixmap.convertFromImage(*newBackground);
}

void MapRenderer::drawTileLayer(QPainter& painter, const std::vector<int>& layer, int startX, int startY, int endX, int endY) {
	for (int y = startY; y < endY; ++y) {
		for (int x = startX; x < endX; ++x) {
			int tile = layer[x + mapWidth * y];

			// TODO Don't hardcode the empty tile id
			if (tile != 255) {
				// TODO Don't hardcode tile size?

				// Position of the tile in the tileset image
				int tx = (tile % 10) * 32;
				int ty = (tile / 10) * 32;

				// TODO Use drawPixmap? That's probably a lot faster, but how am I going to implement animated tiles?
				int drawX = (x - startX) * 32;
				int drawY = (y - startY) * 32;

				painter.drawImage(drawX, drawY, *tilesetImage, tx, ty, 32, 32);
			}
		}
	}
}

void MapRenderer::setViewX(int newX) {
	viewportX = newX;
}

void MapRenderer::setViewY(int newY) {
	viewportY = newY;
}

void MapRenderer::setViewSize(const QSize& newSize) {
	viewportWidth = newSize.width();
	viewportHeight = newSize.height();
	
	backgroundRect.setX(0);
	backgroundRect.setY(0);
	backgroundRect.setSize(newSize);
}

void MapRenderer::drawBackgroundSprites(QPainter& painter, const std::vector<int>& layer, int startX, int startY, int endX, int endY) {
	// TODO FIX THIS
	// Render 6 "tiles" of sprites outside of the view so bigger sprites don't disappear when their base position isn't in view
	int tolerance = 10 * 32;

	if (startX - tolerance >= 0) startX -= tolerance;
	if (startY - tolerance >= 0) startY -= tolerance;

	if (endX + tolerance < mapWidth) endX += tolerance;
	if (endY + tolerance < mapHeight) endY += tolerance;

	for (int y = startY; y < endY; ++y) {
		for (int x = startX; x < endX; ++x) {
			int spriteIndex = layer[x + mapWidth * y];

			// TODO Don't hardcode the empty tile id
			if (spriteIndex != 255) {
				if (spriteIndex >= 0 && spriteIndex < mapData->spriteObjectList.size()) {
					const PK2::SpriteBase* sprite = &mapData->spriteObjectList[spriteIndex];

					if (sprite->type == PK2::SpriteTypes::BACKGROUND) {
						int sprXAdjusted = ((x - startX) * 32) - (sprite->frameWidth / 2) + 16;
						int sprYAdjusted = ((y - startY) * 32) - (sprite->frameHeight - 32);

						painter.drawPixmap(sprXAdjusted, sprYAdjusted, *sprite->image, sprite->frameX, sprite->frameY, sprite->frameWidth, sprite->frameHeight);
					}
				}
			}
		}
	}
}

void MapRenderer::drawForegroundSprites(QPainter& painter, const std::vector<int>& layer, int startX, int startY, int endX, int endY) {
	for (int y = startY; y < endY; ++y) {
		for (int x = startX; x < endX; ++x) {
			int spriteIndex = layer[x + mapWidth * y];

			// TODO Don't hardcode the empty tile id
			if (spriteIndex != 255) {
				if (spriteIndex >= 0 && spriteIndex < mapData->spriteObjectList.size()) {
					const PK2::SpriteBase* sprite = &mapData->spriteObjectList[spriteIndex];

					if (sprite->type != PK2::SpriteTypes::BACKGROUND) {
						int sprXAdjusted = ((x - startX) * 32) - (sprite->frameWidth / 2) + 16;
						int sprYAdjusted = ((y - startY) * 32) - (sprite->frameHeight - 32);

						painter.drawPixmap(sprXAdjusted, sprYAdjusted, *sprite->image, sprite->frameX, sprite->frameY, sprite->frameWidth, sprite->frameHeight);
					}
				}
			}
		}
	}
}

void MapRenderer::drawSpriteHighlights(QPainter& painter, const std::vector<int>& layer, int startX, int startY, int endX, int endY) {
	for (int y = startY; y < endY; ++y) {
		for (int x = startX; x < endX; ++x) {
			int spriteIndex = layer[x + mapWidth * y];

			// TODO Don't hardcode the empty tile id
			if (spriteIndex != 255) {
				if (spriteIndex >= 0 && spriteIndex < mapData->spriteObjectList.size()) {
					painter.setPen(Qt::black);
					painter.drawRect((x - startX) * 32, (y - startY) * 32, 32, 32);

					painter.setPen(Qt::white);
					painter.drawRect((x - startX) * 32 + 1, (y - startY) * 32 + 1, 30, 30);
				}
			}
		}
	}
}

void MapRenderer::setShowSpriteBase(bool show) {
	highlightSpritePositions = show;
}

void MapRenderer::mapDataChanged(const PK2::MapData& newMapData) {
	mapData = &newMapData;
}