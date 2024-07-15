#include "SelectionTool.h"
#include "MapUtil.h"

SelectionTool::SelectionTool() {}

void SelectionTool::draw(QPainter& painter, const QString& tileset, const QString& background) {
    if (isDragging()) {
        int x = selectionRect.x() - getViewX();
        int y = selectionRect.y() - getViewY();

        // Outer rect
        painter.setPen(Qt::black);
        painter.drawRect(x, y, selectionRect.width() - 1, selectionRect.height() - 1);

        // Inner rect
        painter.drawRect(x + 2, y + 2, selectionRect.width() - 5, selectionRect.height() - 5);

        // Middle rect
        painter.setPen(Qt::white);
        painter.drawRect(x + 1, y + 1, selectionRect.width() - 3, selectionRect.height() - 3);
    }
}

void SelectionTool::mousePressed() {
    selectionStart.setX(MapUtil::alignToGrid(getMousePositionMap().x()));
    selectionStart.setY(MapUtil::alignToGrid(getMousePositionMap().y()));

    selectionRect.setRect(selectionStart.x(), selectionStart.y(), 32, 32);
}

void SelectionTool::mouseReleased() {
    if (getMode() == TILE) {
        emit grabTiles(selectionRect);
    } else if (getMode() == SPRITE) {
        emit grabSprites(selectionRect);
    }
}

void SelectionTool::mouseMoved() {
    if (isDragging()) {
        int endX = MapUtil::alignToGrid(getMousePositionMap().x());
        int endY = MapUtil::alignToGrid(getMousePositionMap().y());

        MapUtil::calculateSelectionRect(selectionRect, selectionStart.x(), selectionStart.y(), endX, endY);
    }
}
