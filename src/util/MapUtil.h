#pragma once
#include <QRect>

class MapUtil {
public:
    static constexpr int TILE_SIZE = 32;

    static inline int alignToGrid(int position) {
		return (position / TILE_SIZE) * TILE_SIZE;
	}

	/*
		Turns a map position into a screen position.

		Example:
			X Position on map:		15
			X Position on screen:	480
	*/
    static inline int positionToScreen(int position) {
		return position * TILE_SIZE;
	}

    static inline int positionToMap(int position) {
		return position / TILE_SIZE;
	}

    static void calculateSelectionRect(QRect& selectionRect, int startX, int startY, int endX, int endY) {
        if (startX > endX) {
            int tmpX = endX;

            endX = startX;
            startX = tmpX;

            selectionRect.setX(startX);
        }

        if (startY > endY) {
            int tmpY = endY;

            endY = startY;
            startY = tmpY;

            selectionRect.setY(startY);
        }

        selectionRect.setWidth(endX - startX + 32);
        selectionRect.setHeight(endY - startY + 32);
    }
};
