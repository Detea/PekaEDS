#include "TilesetRenderer.h"
#include <qpainter.h>
#include <qdebug.h>

void TilesetRenderer::draw(QPainter& painter, const QPixmap* tilesetImage, const QRect& selectionRect) {
    drawTileset(painter, tilesetImage);

    //drawSelection(painter, selectionRect);
}

void TilesetRenderer::drawSelection(QPainter& painter, const QRect& selectionRect) {
	// Outer rect
	painter.setPen(Qt::black);
	painter.drawRect(selectionRect.x(), selectionRect.y(), selectionRect.width() - 1, selectionRect.height() - 1);

	// Middle rect
	painter.setPen(Qt::white);
	painter.drawRect(selectionRect.x() + 1, selectionRect.y() + 1, selectionRect.width() - 3, selectionRect.height() - 3);

	// Inner rect
	painter.setPen(Qt::black);
	painter.drawRect(selectionRect.x() + 2, selectionRect.y() + 2, selectionRect.width() - 5, selectionRect.height() - 5);
}

void TilesetRenderer::drawTileset(QPainter& painter, const QPixmap* tilesetImage) {
    painter.fillRect(0, 0, tilesetImage->width(), tilesetImage->height(), Qt::darkGray);

    painter.drawPixmap(0, 0, *tilesetImage);
}
