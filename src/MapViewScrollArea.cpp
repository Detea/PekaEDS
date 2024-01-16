#include "MapViewScrollArea.h"
#include <qscrollbar.h>

MapViewScrollArea::MapViewScrollArea(QWidget* parent) : QWidget(parent) {
	setupUI();
	setupScrollBars();

	connectSlots();
};

void MapViewScrollArea::scrollBarsVisible(bool visible) {
	verticalScrollBar->setVisible(visible);
	horizontalScrollBar->setVisible(visible);
}

void MapViewScrollArea::setupUI() {
	verticalScrollBar = new QScrollBar(Qt::Vertical, this);
	horizontalScrollBar = new QScrollBar(Qt::Horizontal, this);

	gridLayout = new QGridLayout(this);
	gridLayout->setContentsMargins(0, 0, 0, 0);

	gridLayout->addWidget(verticalScrollBar, 0, 1);
	gridLayout->addWidget(horizontalScrollBar, 1, 0);

	gridLayout->setVerticalSpacing(0);
	gridLayout->setHorizontalSpacing(0);

	gridLayout->setColumnStretch(0, 1);
	gridLayout->setColumnStretch(1, 1);

	gridLayout->setRowStretch(1, 1);

	setLayout(gridLayout);
}

void MapViewScrollArea::connectSlots() {
	connect(mapView, &MapView::viewportXChanged, horizontalScrollBar, &QScrollBar::setValue);
	connect(mapView, &MapView::viewportYChanged, verticalScrollBar, &QScrollBar::setValue);

	connect(verticalScrollBar, &QScrollBar::valueChanged, mapView, &MapView::setViewportY);
	connect(horizontalScrollBar, &QScrollBar::valueChanged, mapView, &MapView::setViewportX);
}

int startX = 0;
int startY = 0;

void MapViewScrollArea::mouseMoveEvent(QMouseEvent* e) {
	if (dragging) {
		qDebug() << panStartPosition.x() - e->x() << " - " << panStartPosition.y() - e->y();

		// TODO Fix this
		horizontalScrollBar->setValue(startX - (panStartPosition.x() - e->x()));
		verticalScrollBar->setValue(startY - (panStartPosition.y() - e->y()));
	}
}

void MapViewScrollArea::mousePressEvent(QMouseEvent* e) {
	if (e->button() == Qt::LeftButton) {
		setFocus();
	} else if (e->button() == Qt::MiddleButton) {
		panStartPosition = e->pos();

		panOffset.setX(panStartPosition.x() - horizontalScrollBar->value());
		panOffset.setY(panStartPosition.y() - verticalScrollBar->value());

		startX = horizontalScrollBar->value();
		startY = verticalScrollBar->value();

		dragging = true;
	}

}

void MapViewScrollArea::mouseReleaseEvent(QMouseEvent* e) {
	if (e->button() == Qt::MiddleButton) dragging = false;
}

// TODO Might want to make these keys customizable?
void MapViewScrollArea::keyPressEvent(QKeyEvent* e) {
	switch (e->key()) {
		case Qt::Key_Left:
			moveViewX(-32);
			break;

		case Qt::Key_Right:
			moveViewX(32);
			break;

		case Qt::Key_Up:
			moveViewY(-32);
			break;

		case Qt::Key_Down:
			moveViewY(32);
			break;
	}
}

void MapViewScrollArea::moveViewX(int amount) {
	horizontalScrollBar->setValue(horizontalScrollBar->value() + amount);
}

void MapViewScrollArea::moveViewY(int amount) {
	verticalScrollBar->setValue(verticalScrollBar->value() + amount);
}

void MapViewScrollArea::setViewPosition(int x, int y) {
	if (x < 0 || x > 256 * 32 || y < 0 || y > 224 * 32) return;

	horizontalScrollBar->setValue(x);
	verticalScrollBar->setValue(y);
}

void MapViewScrollArea::setMapView(MapView* newMapView) {
	// TODO Disconnect slots?

	mapView = newMapView;

	gridLayout->addWidget(mapView, 0, 0);

	mapView->setViewportX(0);
	mapView->setViewportY(0);

	connectSlots();
}

void MapViewScrollArea::setupScrollBars() {
	verticalScrollBar->setSingleStep(1);
	horizontalScrollBar->setSingleStep(1);

	// TODO Don't hardcode map size
	verticalScrollBar->setRange(0, 224 * 32);
	horizontalScrollBar->setRange(0, 256 * 32);
}

MapViewScrollArea::~MapViewScrollArea() {

}