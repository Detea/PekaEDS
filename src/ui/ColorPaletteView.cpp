#include "ColorPaletteView.h"
#include <QPainter>

ColorPaletteView::ColorPaletteView(QWidget* parent) : QDialog(parent) {
    setFixedSize(ENTRY_SIZE * ROWS, ENTRY_SIZE * COLS);

    setWindowTitle("Select a color from the palette...");
}

void ColorPaletteView::paintEvent(QPaintEvent* e) {
    QPainter painter(this);

    for (int x = 0; x < ROWS; x++) {
        for (int y = 0; y < COLS; y++) {
            const QList<QRgb>& palette = imageCache->getBackgroundPalette(selectedRegion->backgroundImageFile);

            painter.fillRect(x * ENTRY_SIZE, y * ENTRY_SIZE, ENTRY_SIZE, ENTRY_SIZE, palette.at(x + ROWS * y));
        }
    }

    // Draw selection rect

    int selectionY = (selectedColor / 16) * ENTRY_SIZE;

    // Outer rect
    painter.setPen(Qt::black);
    painter.drawRect(0, selectionY, (ROWS * ENTRY_SIZE) - 1, (ENTRY_SIZE * 2) - 1);

    // Inner rect
    painter.drawRect(2, selectionY + 2, (ROWS * ENTRY_SIZE) - 5, (ENTRY_SIZE * 2) - 5);

    // Middle rect
    painter.setPen(Qt::white);
    painter.drawRect(1, selectionY + 1, (ROWS * ENTRY_SIZE) - 3, (ENTRY_SIZE * 2) - 3);
}

void ColorPaletteView::mousePressEvent(QMouseEvent* e) {
    int xPos = e->pos().x() / ENTRY_SIZE;
    int yPos = e->pos().y() / ENTRY_SIZE;

    selectionX = xPos * ENTRY_SIZE;
    selectionY = yPos * ENTRY_SIZE;

    int color = ((yPos * ROWS + xPos) / ENTRY_SIZE) * ENTRY_SIZE;

    // Only use the first color of each entry in the palette
    selectedColor = color - (color % 32);

    emit selectedColorSet(color);

    update();
}

void ColorPaletteView::open(int newSelectedColor) {
    selectedColor = newSelectedColor;

    if (selectedColor == -1) selectedColor = 0;

    selectionX = (selectedColor % ROWS) * ENTRY_SIZE;
    selectionY = (selectedColor / COLS) * ENTRY_SIZE;

    update();

    QDialog::open();
}

void ColorPaletteView::setImageCache(ImageCache& newImageCache) {
    if (!imageCache) imageCache = &newImageCache;
}

void ColorPaletteView::setSelectedRegion(const Region& newRegion) {
    selectedRegion = &newRegion;

    if (isVisible()) update();
}
