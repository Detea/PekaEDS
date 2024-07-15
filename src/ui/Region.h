#pragma once
#include <qstring.h>
#include <qpixmap.h>

#include <vector>
#include "PK2FileUtil.h"

struct Region {
    int width = 200;
    int height = 200;

    int scrolling = 0;
    int weather = 0;

    int splashColor = -1;   // Index into the color palette or -1

    // These values have to point to the palette. They must be >= 0 <= 255
    int fireColor1 = 64;
    int fireColor2 = 128;

	QString backgroundImageFile;
	QString tilesetImageFile;
    QString tilesetBackgroundImageFile;

	QString musicFile;

    QString editorName;

    int compression = PK2FileUtil::Compression::NONE;

    bool missingBackground = false;

    std::vector<int> backgroundLayer;
    std::vector<int> foregroundLayer;
    std::vector<int> spriteLayer;
};

