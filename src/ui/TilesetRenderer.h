#pragma once
#include <qimage.h>
#include <qrect.h>
#include <qpen.h>

class TilesetRenderer {
public:
    void draw(QPainter& painter, const QPixmap* tilesetImage, const QRect& selectionRect);

private:
	void drawSelection(QPainter& painter, const QRect& selectionRect);
    void drawTileset(QPainter& painter, const QPixmap* tilesetImage);
};

