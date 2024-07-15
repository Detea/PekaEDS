#include "BackgroundRenderer.h"

#include <qpainter.h>

BackgroundRenderer::BackgroundRenderer(QWidget* parent) : QWidget(parent) {

}

void BackgroundRenderer::paintEvent(QPaintEvent* e) {
	QPainter painter(this);

	if (image) {
		int bgRepeatX = viewportSize.width() / image->width();
		int bgRepeatY = viewportSize.height() / image->height();

		for (int bgX = 0; bgX < bgRepeatX + 1; ++bgX) {
			for (int bgY = 0; bgY < bgRepeatY + 1; ++bgY) {
				painter.drawImage(bgX * image->width(), bgY * image->height(), *image);
			}
		}
	}
}

void BackgroundRenderer::backgroundChanged(const QImage* newImage) {
	image = newImage;

	update();
}

void BackgroundRenderer::viewportSizeChanged(const QSize& newSize) {
	viewportSize = newSize;

	update();
}
