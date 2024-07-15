#ifndef COLORPALETTEVIEW_H
#define COLORPALETTEVIEW_H

#include <QDialog>
#include <QMouseEvent>
#include "Region.h"
#include "ImageCache.h"

class ColorPaletteView : public QDialog {
    Q_OBJECT

public:
    ColorPaletteView(QWidget* parent);

    static void setImageCache(ImageCache& newImageCache);
    void setSelectedRegion(const Region& newRegion);

    void open(int selectedColor);

signals:
    void selectedColorSet(int selectedColor);

protected:
    void paintEvent(QPaintEvent* e) override;
    void mousePressEvent(QMouseEvent* e) override;

private:
    inline static constexpr int ENTRY_SIZE = 32;
    inline static constexpr int ROWS = 16;
    inline static constexpr int COLS = 16;
    inline static constexpr int ENTRY_TOTAL = ROWS * COLS;

    inline static ImageCache* imageCache = nullptr;
    inline static const Region* selectedRegion = nullptr;

    int selectionX = 0;
    int selectionY = 0;

    int selectedColor = 0;
};

#endif // COLORPALETTEVIEW_H
