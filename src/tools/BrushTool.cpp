#include "BrushTool.h"
#include "MapUtil.h"

BrushTool::BrushTool() {}

void BrushTool::draw(QPainter& painter, const QString& tileset, const QString& background) {
    if (getMode() == MODE::TILE) {
        for (int y = 0; y < tileSelectionHeight; ++y) {
            for (int x = 0; x < tileSelectionWidth; ++x) {
                int tile = selectedTiles[x + y * tileSelectionWidth];
                int tx = (tile % 10) * 32;
                int ty = (tile / 10) * 32;

                int drawX = MapUtil::alignToGrid(getMousePositionView().x()) + (x * 32);
                int drawY = MapUtil::alignToGrid(getMousePositionView().y()) + (y * 32);

                painter.drawPixmap(drawX, drawY, imageCache->getTileset(tileset, background), tx, ty, 32, 32);
            }
        }
    } else if (getMode() == MODE::SPRITE) {
        if (getSpriteList() && isRegionListValid()) {
            for (int y = 0; y < spriteSelectionHeight; ++y) {
                for (int x = 0; x < spriteSelectionWidth; ++x) {
                    int spriteNumber = selectedSprites[x + y * spriteSelectionWidth];

                    if (spriteNumber != 255 && spriteNumber < getSpriteList()->size()) {
                        const PK2::SpriteBase sprite = getSpriteList()->at(spriteNumber);

                        int sprXAdjusted = MapUtil::alignToGrid(getMousePositionView().x()) + (x * 32) - (sprite.frameWidth / 2) + 16;
                        int sprYAdjusted = MapUtil::alignToGrid(getMousePositionView().y()) + (y * 32) - (sprite.frameHeight - 32);

                        painter.drawPixmap(sprXAdjusted,
                                           sprYAdjusted,

                                           imageCache->getSpriteImage(sprite, getSelectedRegion().backgroundImageFile),

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

void BrushTool::mousePressed() {
    lastMousePosition = getMousePositionMap();

    if (getMode() == Tool::MODE::TILE) {
        emit tilePlaced(getMousePositionMap().x(), getMousePositionMap().y(), tileSelectionWidth, tileSelectionHeight, selectedTiles);
    } else {
        emit spritePlaced(getMousePositionMap().x(), getMousePositionMap().y(), spriteSelectionWidth, spriteSelectionHeight, selectedSprites);
    }
}

void BrushTool::mouseReleased() {}

void BrushTool::mouseMoved() {
    /*
     * TODO [IMPLEMENT] Implement this?
     * Align mousePosition with selectionWidth/Height instead of tilesize?
    const QPoint& mouse = getMousePositionMap();
    int maxThresholdX = tileSelectionWidth * 32;
    int maxThresholdY = tileSelectionHeight * 32;
    int thresholdX = std::max(mouse.x(), lastMousePosition.x()) - std::min(mouse.x(), lastMousePosition.x());
    int thresholdY = std::max(mouse.y(), lastMousePosition.y()) - std::min(mouse.y(), lastMousePosition.y());
    */

    /*&&
                thresholdX >= maxThresholdX ||
                thresholdY >= maxThresholdY)*/
    if (isDragging()) {
        if (getMode() == Tool::MODE::TILE) {
            emit tilePlaced(getMousePositionMap().x(), getMousePositionMap().y(), tileSelectionWidth, tileSelectionHeight, selectedTiles);
        } else {
            emit spritePlaced(getMousePositionMap().x(), getMousePositionMap().y(), spriteSelectionWidth, spriteSelectionHeight, selectedSprites);
        }

        lastMousePosition = getMousePositionMap();
    }
}
