#pragma once
#include <qwidget.h>
#include <qrect.h>

#include "TilesetRenderer.h"
#include "PekaEDSWindow.h"

class PekaEDSWindow;

class TilesetView : public QWidget {
	Q_OBJECT

public:
	TilesetView(const PekaEDSWindow* mainWindow);

	// TODO Don't think this is actually needed?
	void setRenderer(const TilesetRenderer* newRenderer);

protected:
	// TODO Create MouseHandlers for all components? TilesetMouseHandler, MapMouseHandler
	void mousePressEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;

	void paintEvent(QPaintEvent* event) override;

public slots:
	void tilesetImageChanged(const QImage* newTileset);

private:
	void connectSlots();

private:
	int tilesetWidth = 0;
	int tilesetHeight = 0;

	const PekaEDSWindow* mainWindow = nullptr;

	const QImage* tilesetImage = nullptr;

	QRect selectionRect;
	TilesetRenderer renderer;
};