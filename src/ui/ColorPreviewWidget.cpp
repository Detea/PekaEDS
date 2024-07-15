#include "ColorPreviewWidget.h"
#include <QPainter>

ColorPreviewWidget::ColorPreviewWidget(QWidget *parent) : QWidget{parent} {
    setFixedSize(24, 24);

    colorPaletteView = new ColorPaletteView(this);

    connect(colorPaletteView, &ColorPaletteView::selectedColorSet, this, &ColorPreviewWidget::selectedColorChanged);
}

void ColorPreviewWidget::paintEvent(QPaintEvent* e) {
    QPainter painter(this);

    if (imageCache && selectedColorIndex >= 0 && selectedColorIndex <= 255) {
        QColor color = imageCache->getBackgroundPalette(selectedRegion->backgroundImageFile)[selectedColorIndex];
        painter.fillRect(0, 0, width(), height(), color);
    }

    painter.setPen(Qt::black);
    painter.drawRect(0, 0, width() - 1, height() - 1);
}

void ColorPreviewWidget::setImageCache(ImageCache& newImageCache) {
    ColorPaletteView::setImageCache(newImageCache);

    imageCache = &newImageCache;
}

void ColorPreviewWidget::selectedRegionChanged(const Region& newRegion) {
    selectedRegion = &newRegion;

    colorPaletteView->setSelectedRegion(newRegion);

    update();
}

void ColorPreviewWidget::setSelectedColor(int index) {
    selectedColorIndex = index;

    //colorPaletteView->setSelectedColor(index);

    update();
}

void ColorPreviewWidget::mousePressEvent(QMouseEvent* e) {
    colorPaletteView->open(selectedColorIndex);
}
