#include "SpriteListView.h"
#include <qpainter.h>
#include <QtGlobal>

SpriteListView::SpriteListView() {
	setupUI();
}

void SpriteListView::setupUI() {
	// This size is arbitrary. This widget needs to have it's minimum size set to something in order to work correctly with it's owning QScrollArea.
	// TODO Change this when switching to expanded mode?
	setMinimumSize(ENTRY_WIDTH_COMPACT, ENTRY_HEIGHT_COMPACT);

	stPlayer.setText("Player");

	fontCurrent = font();
	fontBold = QFont(font().family(), font().pointSize(), QFont::Bold);
}

/*
	NOTE
		Make sure to update this widgets size when adding or removing sprites
*/

void SpriteListView::paintEvent(QPaintEvent* e) {
	QPainter painter(this);

	// TODO Style this widget with CSS
	painter.fillRect(0, 0, width(), height(), palette().color(QPalette::Base));

    if (sprites && !sprites->empty() && regionList) {
		if (playerTextWidth == 0) {
			// NOTE: This doesn't update when the font changes!
			playerTextWidth = painter.fontMetrics().boundingRect(stPlayer.text()).width();
		}

        if (selectedIndex != -1) {
            painter.fillRect(0, selectedIndex * ENTRY_HEIGHT_COMPACT, width(), ENTRY_HEIGHT_COMPACT, palette().color(QPalette::Highlight));

            painter.setPen(palette().color(QPalette::Highlight).lighter());
            painter.drawRect(0, selectedIndex * ENTRY_HEIGHT_COMPACT, width(), ENTRY_HEIGHT_COMPACT);
        }

        // Divider line between sprite image and data
		painter.setPen(Qt::lightGray);
        painter.drawLine(IMAGE_MAX_WIDTH_COMPACT + IMAGE_OFFSET_X + 1, 0, IMAGE_MAX_WIDTH_COMPACT + IMAGE_OFFSET_X + 1, height());
		painter.setPen(Qt::black);

		for (int i = 0; i < sprites->size(); ++i) {
			painter.setPen(Qt::lightGray);

            // TODO Draw line after last sprite (when searching)
            painter.drawLine(0, i * ENTRY_HEIGHT_COMPACT, width(), i * ENTRY_HEIGHT_COMPACT);

			int spriteWidth = sprites->at(i).frameWidth;
			int spriteHeight = sprites->at(i).frameHeight;

            PK2::SpriteBase spr = sprites->at(i);

            // TODO OPTIMIZATION Cache these scaled down versions
			if (spriteWidth > IMAGE_MAX_WIDTH_COMPACT) {
                spriteHeight = IMAGE_MAX_WIDTH_COMPACT / (spriteWidth / spriteHeight);
                spriteWidth = IMAGE_MAX_WIDTH_COMPACT;
            }

			if (spriteHeight > IMAGE_MAX_HEIGHT_COMPACT) {
                spriteWidth = IMAGE_MAX_HEIGHT_COMPACT / (spriteHeight / spriteWidth);
                spriteHeight = IMAGE_MAX_HEIGHT_COMPACT;
			}

			// Position the sprite's image in the center of the preview area
			int spriteX = IMAGE_OFFSET_X + (IMAGE_MAX_WIDTH_COMPACT / 2) - (spriteWidth / 2);
			int spriteY = (IMAGE_OFFSET_Y + (i * ENTRY_HEIGHT_COMPACT)) + (IMAGE_MAX_HEIGHT_COMPACT / 2) - (spriteHeight / 2);

			painter.drawPixmap(spriteX,
							   spriteY,
							   spriteWidth,
							   spriteHeight,

                               imageCache->getSpriteImage(spr, regionList->at(selectedRegionIndex).backgroundImageFile),
                               spr.frameX,
                               spr.frameY,
                               spr.frameWidth,
                               spr.frameHeight);

			//painter.setPen(Qt::black);
			//painter.drawRect(IMAGE_OFFSET_X, IMAGE_OFFSET_Y + (i * ENTRY_HEIGHT_COMPACT), IMAGE_MAX_WIDTH_COMPACT, IMAGE_MAX_HEIGHT_COMPACT);

            painter.setPen(Qt::black);
			painter.setFont(fontBold);
			painter.drawText(NAME_X, NAME_Y + (i * ENTRY_HEIGHT_COMPACT), sprites->at(i).filename);
			painter.setFont(fontCurrent);

			// TODO Should probably use StaticText for name and filename, those don't change and can be reused
			painter.drawText(NAME_X, NAME_Y + (i * ENTRY_HEIGHT_COMPACT) + 15, sprites->at(i).name);

			// TODO IMPORTANT! Check if type is in bounds? Check at load time?
			painter.drawText(NAME_X, (i * ENTRY_HEIGHT_COMPACT) + ENTRY_HEIGHT_COMPACT - 5, typesAsString[sprites->at(i).type]);
            painter.drawText(NAME_X, (i * ENTRY_HEIGHT_COMPACT) + ENTRY_HEIGHT_COMPACT - 20, "Placed: " + QString::number(sprites->at(i).placementAmount));

			if (sprites->at(i).player) {
				painter.drawStaticText(width() - playerTextWidth - 10, IMAGE_OFFSET_Y + (i * ENTRY_HEIGHT_COMPACT), stPlayer);
			}

			// TODO Draw filename at width() - filename.width() - use StaticText?
			// TODO sprite.type to string
			//painter.drawText(NAME_X, NAME_Y + (i * ENTRY_HEIGHT_COMPACT) + 25, QString::fromStdString(sprites->at(i).type));
        }

        // Draw the repositioning line
        if (dragging) {
            int lineY = mousePosition.y();

            lineY = (lineY / ENTRY_HEIGHT_COMPACT) * ENTRY_HEIGHT_COMPACT;

            painter.setPen(Qt::black);

            // Middle line
            painter.drawLine(0, lineY, width(), lineY -1);
            painter.drawLine(0, lineY, width(), lineY);
            painter.drawLine(0, lineY, width(), lineY + 1);

            // Left line
            painter.drawLine(0, lineY - 5, 0, lineY + 5);
            painter.drawLine(1, lineY - 3, 1, lineY + 3);

            // Right line
            painter.drawLine(width() - 1, lineY - 5, width() - 1, lineY + 5);
            painter.drawLine(width() - 2, lineY - 3, width() - 2, lineY + 3);
        }
    } else {
        painter.drawText(20, 20, "No sprites found!");
    }
}

void SpriteListView::mousePressEvent(QMouseEvent* event) {
    if (sprites) {
        mouseClickPosition = event->pos();

        selectedIndex = mouseClickPosition.y() / ENTRY_HEIGHT_COMPACT;

        if (selectedIndex >= sprites->size()) {
            selectedIndex = sprites->size() - 1;
        }

        emit selectedSpriteChanged(selectedIndex);

        update();
    }
}

void SpriteListView::mouseMoveEvent(QMouseEvent* event) {
    if (sprites) {
        mousePosition = event->pos();

        if (!dragging && allowSpriteReordering) {
            QPoint length = mousePosition - mouseClickPosition;
            if (length.manhattanLength() > 30) {
                selectedIndexDragging = mouseClickPosition.y() / ENTRY_HEIGHT_COMPACT;

                if (selectedIndexDragging < sprites->size()) {
                    dragging = true;

                    qDebug() << selectedIndexDragging << sprites->size();
                } else {
                    selectedIndexDragging = -1;
                    dragging = false;
                }
            }
        } else {
            update();
        }
    }
}

void SpriteListView::mouseReleaseEvent(QMouseEvent* event) {
    if (sprites) {
        if (dragging && selectedIndexDragging != -1) {
            int targetIndex = mousePosition.y() / ENTRY_HEIGHT_COMPACT;

            PK2::SpriteBase draggedSprite = sprites->at(selectedIndexDragging);
            sprites->erase(sprites->begin() + selectedIndexDragging);
            sprites->insert(sprites->begin() + targetIndex, draggedSprite);

            selectedIndex = targetIndex;
            selectedIndexDragging = -1;

            dragging = false;

            update();
        }
    }
}

void SpriteListView::setSpriteList(std::vector<PK2::SpriteBase>* newSpriteList) {
    sprites = newSpriteList;

    if (!sprites->empty()) {
        setFixedSize(ENTRY_WIDTH_COMPACT, ENTRY_HEIGHT_COMPACT * sprites->size());
    } else {
        setFixedSize(ENTRY_WIDTH_COMPACT, ENTRY_HEIGHT_COMPACT);
    }
}

void SpriteListView::updateList() {
    if (sprites) {
        setFixedSize(ENTRY_WIDTH_COMPACT, ENTRY_HEIGHT_COMPACT * sprites->size());

        update();
    }
}

void SpriteListView::setSelectedIndex(int newIndex) {
    selectedIndex = newIndex;
}

void SpriteListView::setImageCache(ImageCache& imageCache) {
    this->imageCache = &imageCache;
}

void SpriteListView::spritePixmapListChanged(const std::vector<QPixmap*>* newList) {
    spritePixmaps = newList;
}

void SpriteListView::selectedRegionChanged(int regionIndex) {
    currentRegion = &regionList->at(regionIndex);
    selectedRegionIndex = regionIndex;

    update();
}

void SpriteListView::regionsLoaded(const std::vector<Region>* newRegionList) {
    regionList = newRegionList;
}

int SpriteListView::getSelectedIndex() {
    if (selectedIndex < 0) selectedIndex = 0;
    if (selectedIndex > sprites->size()) selectedIndex = sprites->size() - 1;

	return selectedIndex;
}

void SpriteListView::clearSelection() {
    selectedIndex = -1;
}

void SpriteListView::setAllowSpriteReordering(bool allow) {
    allowSpriteReordering = allow;
}
