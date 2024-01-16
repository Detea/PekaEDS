#pragma once
#include <qwidget.h>

#include <qevent.h>

#include <qrect.h>

#include "PekaEDSWindow.h"
#include "MapRenderer.h"
#include "PK2MapData.h"

class PekaEDSWindow;

class MapView : public QWidget {
	Q_OBJECT
public:
	MapView(const PekaEDSWindow* mainWindow);

public slots:
	void setViewportX(int newX);
	void setViewportY(int newY);

signals:
	void viewportXChanged(int newX);
	void viewportYChanged(int newY);
	void viewportSizeChanged(const QSize& newSize);

protected:
	void mousePressEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;

	void resizeEvent(QResizeEvent* e) override;

	void paintEvent(QPaintEvent* event) override;

	// Should do this, the rest of the class should suffice
	// void setMapRenderer(MapRenderer* mapRenderer);
 
private slots:

private:
	void connectSlots();

	void setupUI();

private:
	const PekaEDSWindow* mainWindow = nullptr;

	QRect view;

	// MapRenderer should be changeable via dependency injection
	MapRenderer mapRenderer;
};

