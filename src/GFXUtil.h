#pragma once
#include <qimage.h>

namespace GFXUtil {
	void recolorSprite(QImage* image, int paletteColor) {
		for (int y = 0; y < image->height(); ++y) {
			uchar* scanLine = image->scanLine(y);

			for (int x = 0; x < image->width(); ++x) {
				uchar& pixel = scanLine[x];

				if (pixel != 255) {
					pixel %= 32;
					pixel += paletteColor;
				}
			}
		}
	}

	void makeTransparent(QImage* image) {
		const QRgb alphaColor = image->colorTable()[255];
		image->setAlphaChannel(image->createMaskFromColor(alphaColor, Qt::MaskOutColor));
	}
}