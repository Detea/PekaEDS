#ifndef BRUSHTOOL_H
#define BRUSHTOOL_H

#include "Tool.h"

class BrushTool : public Tool {
    Q_OBJECT

public:
    BrushTool();

    void draw(QPainter& painter, const QString& tileset, const QString& background) override ;

    void mousePressed() override;
    void mouseReleased() override;
    void mouseMoved() override;

private:
    // Uses the width and height of the selection as the grid, instead of the standard 32 by 32 tile size.
    bool useSelectionAsGrid = true;

    QPoint lastMousePosition;
};

#endif // BRUSHTOOL_H
