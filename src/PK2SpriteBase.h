#pragma once

#include <string>
#include <string_view>
#include <qpixmap.h>

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
		/*
			This pointer points to an entry in the spriteImages map in MapHandler

			To draw a sprite use the frameX, Y, Width, Height data to draw a subimage of this pixmap
		*/
		const QPixmap* image = nullptr;

		std::string imageFile;
		std::string name;

		unsigned char color = 0;

		// Only need to know if its a background sprite for rendering and if its a character so that the user can set it as the player sprite
		int type;

		/*
			Frame data for the first animation frame.
		*/
		int frameX = 0;
		int frameY = 0;
		int frameWidth = 0;
		int frameHeight = 0;
	};
}

