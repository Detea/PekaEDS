#include "TilesetView.h"
#include <qpainter.h>
#include <qevent.h>
#include "MapUtil.h"

TilesetView::TilesetView(const PekaEDSWindow* parentWindow) : QWidget(parentWindow->parentWidget()), mainWindow(parentWindow) {
	setMinimumSize(320, 480);

	connectSlots();
}

void TilesetView::setRenderer(const TilesetRenderer* newRenderer) {
	//renderer = newRenderer;
}

void TilesetView::connectSlots() {
	connect(mainWindow, &PekaEDSWindow::tilesetImageChanged, this, &TilesetView::tilesetImageChanged);
}

void TilesetView::paintEvent(QPaintEvent* event) {
	QPainter painter(this);

	if (tilesetImage) renderer.draw(painter, tilesetImage, selectionRect);
}

void TilesetView::mousePressEvent(QMouseEvent* event) {
	int mx = event->x();
	int my = event->y();

	if (mx >= 0 && mx <= tilesetWidth &&
		my >= 0 && my <= tilesetHeight) { 
		selectionRect.setX(PK2::MapUtil::alignToGrid(mx));
		selectionRect.setY(PK2::MapUtil::alignToGrid(my));

		selectionRect.setWidth(PK2::MapUtil::TILE_SIZE);
		selectionRect.setHeight(PK2::MapUtil::TILE_SIZE);
	}

	repaint();
}

void TilesetView::mouseMoveEvent(QMouseEvent* event) {
}

void TilesetView::tilesetImageChanged(const QImage* newTileset) {
	tilesetImage = newTileset;

	repaint();

	if (tilesetWidth != newTileset->width()) {
		tilesetWidth = newTileset->width();
	}

	if (tilesetHeight != newTileset->height()) {
		tilesetHeight = newTileset->height();
	}

	if (width() != tilesetImage->width() || height() != tilesetImage->height()) {
		setFixedSize(tilesetImage->width(), tilesetImage->height());
	}
}