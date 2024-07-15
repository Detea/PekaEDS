#ifndef SELECTIONTOOL_H
#define SELECTIONTOOL_H

#include "Tool.h"

class SelectionTool : public Tool {
    Q_OBJECT

public:
    SelectionTool();

    void draw(QPainter& painter, const QString& tileset, const QString& background) override;

    void mousePressed() override;
    void mouseReleased() override;
    void mouseMoved() override;

signals:
    void grabTiles(const QRect& selectionRect);
    void grabSprites(const QRect& selectionRect);

private:
    QPoint selectionStart;
    QRect selectionRect;
};

#endif // SELECTIONTOOL_H
