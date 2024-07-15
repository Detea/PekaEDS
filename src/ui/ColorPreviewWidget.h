#ifndef COLORPREVIEWWIDGET_H
#define COLORPREVIEWWIDGET_H

#include <QWidget>
#include "ColorPaletteView.h"
#include "Region.h"

class ColorPreviewWidget : public QWidget {
    Q_OBJECT

public:
    explicit ColorPreviewWidget(QWidget *parent = nullptr);

    static void setImageCache(ImageCache& newImageCache);
    void selectedRegionChanged(const Region& newRegion);

signals:
    void selectedColorChanged(int newColorIndex);

public slots:
    void setSelectedColor(int index);

protected:
    void paintEvent(QPaintEvent* e) override;
    void mousePressEvent(QMouseEvent* e) override;

private:
    ColorPaletteView* colorPaletteView = nullptr;
    inline static ImageCache* imageCache = nullptr;
    inline static const Region* selectedRegion = nullptr;

    int selectedColorIndex = 0;
};

#endif // COLORPREVIEWWIDGET_H
