#include "MapView.h"
#include "LayerLayout.h"

#include <qdebug.h>

#include <QResizeEvent>

MapView::MapView(const PekaEDSWindow* mainWindow) : mainWindow(mainWindow) {
	setupUI();
	connectSlots();
}

void MapView::mousePressEvent(QMouseEvent* event) {
	qDebug() << "MapView::mousePressEvent";

	/*
		Only pass the event on when the user presses the middle mouse button, so that they can move the scroll area with the mouse.
		Only pass the event on when the user presses the left mouse button, so that the MapViewScrollArea behind this widget can receive focus, which is needed for scrolling with the arrow keys.
	*/
	if (event->button() == Qt::MiddleButton ||
		event->button() == Qt::LeftButton) {
		event->ignore();
	}
}

void MapView::mouseMoveEvent(QMouseEvent* event) {

	// Only pass the event on when the user presses the middle mouse button, so that they can move the scroll area with the mouse.
	event->ignore();
}

void MapView::mouseReleaseEvent(QMouseEvent* event) {

	if (event->button() == Qt::MiddleButton) event->ignore();
}

void MapView::setupUI() {
}

void MapView::paintEvent(QPaintEvent* event) {
	QPainter painter(this);

	mapRenderer.draw(painter);
}

void MapView::resizeEvent(QResizeEvent* e) {
	mapRenderer.setViewSize(e->size());

	emit viewportSizeChanged(e->size());
}

void MapView::connectSlots() {
	connect(mainWindow, &PekaEDSWindow::tilesetImageChanged, &mapRenderer, &MapRenderer::tilesetChanged);
	connect(mainWindow, &PekaEDSWindow::backgroundImageChanged, &mapRenderer, &MapRenderer::backgroundChanged);

	connect(mainWindow, &PekaEDSWindow::mapDataChanged, &mapRenderer, &MapRenderer::mapDataChanged);
}

void MapView::setViewportX(int newX) {
	int oldX = view.x();
	view.setX(newX);
	
	mapRenderer.setViewX(newX);

	if (oldX != newX) emit viewportXChanged(newX);

	update();
}

void MapView::setViewportY(int newY) {
	int oldY = view.y();
	view.setY(newY);

	mapRenderer.setViewY(newY);

	if (oldY != newY) emit viewportYChanged(newY);

	update();
}