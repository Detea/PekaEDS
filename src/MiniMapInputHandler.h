#pragma once
#include <qevent.h>
#include <qobject.h>

class MiniMapInputHandler : public QObject {
	Q_OBJECT

public:
	MiniMapInputHandler();

	void setMapSize(int width, int height);

	void mousePressEvent(QMouseEvent* event);
	void mouseMoveEvent(QMouseEvent* event);

public slots:
	void setViewportX(int newX);
	void setViewportY(int newY);

	void setViewportSize(const QSize& size);

signals:
	void viewportXChanged(int newX);
	void viewportYChanged(int newY);

private:
	void moveViewport(int newX, int newY);

private:
	int mapWidth = 0;
	int mapHeight = 0;

	// TODO Should I just pass the MiniMaps viewport rect?
	QSize viewportSize;
};

