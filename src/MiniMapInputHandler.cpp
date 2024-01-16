#include "MiniMapInputHandler.h"
#include <qpoint.h>

MiniMapInputHandler::MiniMapInputHandler() {}

void MiniMapInputHandler::mousePressEvent(QMouseEvent* e) {
	if (e->button() == Qt::LeftButton) {
		moveViewport(e->x(), e->y());
	}
}

void MiniMapInputHandler::mouseMoveEvent(QMouseEvent* e) {
	if (e->buttons() & Qt::LeftButton) {
		moveViewport(e->x(), e->y());
	}
}

void MiniMapInputHandler::moveViewport(int newX, int newY) {
	QPoint newPos;

	int halfViewportWidth = viewportSize.width() / 2;
	int halfViewportHeight = viewportSize.height() / 2;

	newPos.setX(newX - halfViewportWidth);
	newPos.setY(newY - halfViewportHeight);

	if (newPos.x() < 0) newPos.setX(0);
	if (newPos.x() + viewportSize.width() >= mapWidth) newPos.setX(mapWidth - viewportSize.width() - 1);	// Subtracting 1 so that the right and bottom sides of the selection rectangle still appear

	if (newPos.y() < 0) newPos.setY(0);
	if (newPos.y() + viewportSize.height() >= mapHeight) newPos.setY(mapHeight - viewportSize.height() - 1);

	// These values get multiplied by 32 because the MiniMapView::setViewportX/Y slots also get used by MapView when its viewport position changes. This is used to keep the scroll bars and minimap in sync
	emit viewportXChanged(newPos.x() * 32);
	emit viewportYChanged(newPos.y() * 32);
}

void MiniMapInputHandler::setViewportX(int newX) {}

void MiniMapInputHandler::setViewportY(int newY) {}

void MiniMapInputHandler::setViewportSize(const QSize& size) {
	viewportSize = size;
}

void MiniMapInputHandler::setMapSize(int width, int height) {
	mapWidth = width;
	mapHeight = height;
}