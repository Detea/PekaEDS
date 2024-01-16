#pragma once

#include <qwidget.h>
#include <qscrollbar.h>
#include <qgridlayout.h>
#include <qpoint.h>

#include "MapView.h"

class MapView;

/*
*	This program uses a custom scroll area component because it only draws tiles and sprites that are visible in the viewport.
*	This could also be achieved by using a regular QScrollArea, but then the size of the MapView Widget would have to be (256x32, 224x32) map width x 32, height x 32, which would be 8192, 7168.
*	Creating a QWidget that large without actually using all that space would be unnecessary. The MapView only uses the space that's visible in the viewport (MapView's width and height).
*/
class MapViewScrollArea : public QWidget {
	Q_OBJECT

public:
	MapViewScrollArea(QWidget* parent);
	~MapViewScrollArea();

	// Turn on/off the scrollbars, used for the distraction free mode.
	void scrollBarsVisible(bool visible);

	void setMapView(MapView* newMapView);

protected:
	void mouseMoveEvent(QMouseEvent* e) override;
	void mousePressEvent(QMouseEvent* e) override;
	void mouseReleaseEvent(QMouseEvent* e) override;

	void keyPressEvent(QKeyEvent* event) override;

private:
	void setupUI();
	void setupScrollBars();

	void connectSlots();

	void moveViewX(int amount);
	void moveViewY(int amount);

	void setViewPosition(int x, int y);

private:
	MapView* mapView = nullptr;

	QScrollBar* verticalScrollBar = nullptr;
	QScrollBar* horizontalScrollBar = nullptr;

	QGridLayout* gridLayout = nullptr;

	QPoint panStartPosition;
	QPoint panOffset;

	bool dragging = false;
};