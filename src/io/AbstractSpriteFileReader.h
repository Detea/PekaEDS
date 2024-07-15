#pragma once
#include <qstring.h>

#include "PK2SpriteBase.h"

/*
	This editor only needs to read certain information about sprites, it doesn't save them.
	That's why this class is called SpriteFileReader instead of handler, like the MapFileHandler.
*/
class AbstractSpriteFileReader {
	/*
		This method reads the following data from the sprite file "filename":
		- image file
		- frame x, y
		- frame width, height

		That's all the editor needs to display a sprite.
	*/
	virtual void read(const QString& filename, PK2::SpriteBase* sprite) = 0;
};