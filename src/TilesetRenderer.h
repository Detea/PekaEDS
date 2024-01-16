#pragma once
#include <qimage.h>
#include <qrect.h>
#include <qpen.h>

class TilesetRenderer {
public:
	void draw(QPainter& painter, const QImage* tilesetImage, const QRect& selectionRect);

private:
	void drawSelection(QPainter& painter, const QRect& selectionRect);
	void drawTileset(QPainter& painter, const QImage& tilesetImage);
};

