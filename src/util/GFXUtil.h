#pragma once
#include <qimage.h>

#include <QDebug>

class GFXUtil {
public:
    static void recolorSprite(QImage& image, int paletteColor) {
        for (int y = 0; y < image.height(); ++y) {
            uchar* scanLine = image.scanLine(y);

            for (int x = 0; x < image.width(); ++x) {
				uchar& pixel = scanLine[x];

				if (pixel != 255) {
					pixel %= 32;
					pixel += paletteColor;
				}
			}
		}
	}

    static void setPalette(const QList<QRgb>& palette, QImage& targetImage) {
        targetImage.setColorTable(palette);
    }

	static void makeTransparent(QImage* image) {
        const QRgb alphaColor = image->colorTable().at(255);
		image->setAlphaChannel(image->createMaskFromColor(alphaColor, Qt::MaskOutColor));
	}

    static void calculateTileColors(const QImage& tilesetImage, QList<QRgb>& tileColors) {
        for (int tile = 0; tile < 150; tile++) {
            // The games color palette technically has 7 primary colors, because the colors after those have special purposes
            unsigned long primaryColors[8];
            for (int i = 0; i < 8; i++) {
                primaryColors[i] = 0;
            }

            unsigned long primaryColor = 0;		// Primary color in the palette, will be the first entry for the specific color in the palette

            /*
            averageShadow contains the "shade" of the color, a value relative to primaryColor.

            Note: All these values are indices in the color palette.
            Let's say the tile has a primary color value of 64 and let's assume that that primary color is used as a red.
            The tile contains pixels that use the color index 73.
            The averageShadow is calculated by this: tilePixel % 32 (see below in the loop)
            73 % 32 = 9

            So the averageShadow for this pixel would be 9.
        */
            unsigned long averageShadow = 0;

            // Gets increased if tilePixel (see below in loop) is one of the regular primary colors
            unsigned long tileNumber = 0;

            // Position of 'tile' in the tileset, used as offset for the next loop
            int tileX = tile % 10;
            int tileY = tile / 10;

            // Loop through all the pixels of the tile (whose index is stored in 'tile') and calculate the average color(shadow?)
            for (int xx = 0; xx < 32; xx++) {
                for (int yy = 0; yy < 32; yy++) {
                    // This contains an index into the color palette, because we're working with a 256 color bitmap
                    unsigned char tilePixel = (unsigned char) tilesetImage.pixelIndex((xx + (tileX * 32)), (yy + (tileY * 32)));

                    // The colors in the palette at entries 224 and above have special purposes and are ignored
                    if (tilePixel < 224) {
                        primaryColor = tilePixel / 32;	// Get the primary color of the pixel
                        primaryColors[primaryColor] = primaryColors[primaryColor] + 1;	// Color has been used, so increase it's count by one

                        // Calculate the distance from the primaryColor value
                        // Each color in the palette has 32 "shades"
                        averageShadow = averageShadow + (tilePixel % 32);

                        tileNumber++;
                    }
                }
            }

            int highestPrimaryColor = 0;

            // Loop through the primary color and find the color that's used the most in the tile
            for (int i = 0; i < 8; i++) {
                unsigned long lastPrimaryColor = 0;

                if (primaryColors[i] > lastPrimaryColor) {
                    highestPrimaryColor = i;

                    lastPrimaryColor = primaryColors[i];
                }
            }

            // If color is valid (meaning it doesn't have a special purpose) its averageShadow gets calculated
            if (tileNumber > 0) {
                averageShadow = averageShadow / tileNumber;
            } else {
                averageShadow = 0;
            }

            if (tile < 90) {
                averageShadow += 3;
            }

            static constexpr unsigned char TILE_SWITCH2 = 146;
            static constexpr unsigned char TILE_SWITCH3 = 147;
            static constexpr unsigned char TILE_SWITCH1 = 145;
            static constexpr unsigned char TILE_START = 148;
            static constexpr unsigned char TILE_END = 149;
            if (tile == TILE_SWITCH1 ||
                tile == TILE_SWITCH2 ||
                tile == TILE_SWITCH3 ||
                tile == TILE_START ||
                tile == TILE_END) {
                averageShadow += 12;
            }

            // Each color in the palette has 32 shades, so we keep clamp averageShadow to that if necessary
            if (averageShadow > 31) {
                averageShadow = 31;
            }

            //tileColor[tile] = averageShadow + highestPrimaryColor * 32;
            tileColors.push_back(averageShadow + highestPrimaryColor * 32);
        }
    }
};
