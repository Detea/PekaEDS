#ifndef MAPPOSITIONDIALOG_H
#define MAPPOSITIONDIALOG_H

#include <QDialog>
#include "GamePaths.h"

class MapPositionDialog : public QDialog {
    Q_OBJECT

public:
    MapPositionDialog(QWidget* parent);
    virtual ~MapPositionDialog();

    void setGamePaths(const GamePaths& newGamePaths);

    void show();

public slots:
    void updateX(int newX);
    void updateY(int newY);
    void updateIconNumber(int newIcon);

signals:
    void positionXChanged(int newX);
    void positionYChanged(int newY);

protected:
    void paintEvent(QPaintEvent *e) override;
    void mousePressEvent(QMouseEvent *e) override;

private:
    static inline constexpr int ICON_WIDTH = 27;
    static inline constexpr int ICON_HEIGHT = 27;

    const GamePaths* gamePaths;

    QImage* mapImage = nullptr;
    QImage pk2stuffImage;

    int oldX = 0;
    int oldY = 0;

    int posX = 0;
    int posY = 0;
    int iconNumber = 0;

    bool loadedFromEpisode = false;
};

#endif // MAPPOSITIONDIALOG_H
