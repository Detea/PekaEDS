#pragma once

#include <qstring.h>
#include <qpixmap.h>

#include <vector>

namespace PK2 {
	/*
		This class only responsibility is to hold data, it does NOT operate on any data.

		This class contains only what the level editor needs to know about:
			- The sprites image file name
			- The x and y position of the first animation frame
			- The width and height of the first animation frame
			- The offset in the color palette which is used to recolor the sprite
	*/
	struct SpriteBase {
		QString imageFile;
        QString imageFileWithColor;
		QString filename;
		QString name;

		unsigned char color = 0;

		// Only need to know if its a background sprite for rendering and if its a character so that the user can set it as the player sprite
        int type = 0;

		bool player = false;

		/*
			Frame data for the first animation frame.
		*/
		int frameX = 0;
		int frameY = 0;
		int frameWidth = 0;
		int frameHeight = 0;

        // Stores how often the sprite has been placed on the map
        int placementAmount = 0;

		inline SpriteBase& operator=(const SpriteBase& other) {
			if (this == &other) {
				return *this;
			}

			imageFile = other.imageFile;
            imageFileWithColor = other.imageFileWithColor;
			filename = other.filename;
			name = other.name;

			color = other.color;

			type = other.type;

			player = other.player;

			frameX = other.frameX;
			frameY = other.frameY;
			frameWidth = other.frameWidth;
			frameHeight = other.frameHeight;

            placementAmount = other.placementAmount;

			return *this;
		}
	};
}

