#pragma once
#include <qwidget.h>
#include <qimage.h>
#include <qevent.h>

/*
	This class is responsible for drawing the background.

	The drawing of the background has been separated into this class to avoid having to repaint the background images, when the viewport position or map changes.

	The background image only changes when the user changes it, so this class only has to repaint when that happens.
*/
class BackgroundRenderer : public QWidget {
public:
	BackgroundRenderer(QWidget* parent);

public slots:
	void backgroundChanged(const QImage* newImage);
	void viewportSizeChanged(const QSize& newSize);

protected:
	void paintEvent(QPaintEvent* e) override;

private:
	const QImage* image = nullptr;

	QSize viewportSize;
};

