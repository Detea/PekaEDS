#include "MapPositionDialog.h"
#include <QPainter>
#include <QMouseEvent>
#include "GFXUtil.h"

MapPositionDialog::MapPositionDialog(QWidget* parent) : QDialog(parent) {
    setFixedSize(640, 480);
    setWindowTitle("Set the level position on the map...");
}

void MapPositionDialog::setGamePaths(const GamePaths& newGamePaths) {
    gamePaths = &newGamePaths;

    pk2stuffImage.load(gamePaths->getPK2StuffFile());
    GFXUtil::makeTransparent(&pk2stuffImage);
}

void MapPositionDialog::show() {
    // Check if the user has loaded an episode and update the map image to the one in the episode folder, if it exists.
    // Check if it has been loaded from the episode folder already to avoid reloading it everytime the dialog is opened.
    if (mapImage && !gamePaths->currentEpisodeFolder().isEmpty() && !loadedFromEpisode) {
        mapImage->load(gamePaths->getMapImage());

        loadedFromEpisode = true;
    } else if (!mapImage) {
        mapImage = new QImage(gamePaths->getMapImage());
    }

    QDialog::show();
}

void MapPositionDialog::paintEvent(QPaintEvent* e) {
    if (mapImage) {
        QPainter painter(this);

        painter.drawImage(0, 0, *mapImage);

        painter.drawImage(posX - 9, posY - 14, pk2stuffImage, 1 + (iconNumber * (ICON_WIDTH + 1)), 452, ICON_WIDTH, ICON_HEIGHT);
    }
}

void MapPositionDialog::mousePressEvent(QMouseEvent* e) {
    oldX = posX;
    oldY = posY;

    posX = e->pos().x();
    posY = e->pos().y();

    emit positionXChanged(posX);
    emit positionYChanged(posY);

    update();
}

void MapPositionDialog::updateX(int newX) {
    oldX = posX;
    posX = newX;

    update();
}

void MapPositionDialog::updateY(int newY) {
    oldY = posY;
    posY = newY;

    update();
}

void MapPositionDialog::updateIconNumber(int newIcon) {
    iconNumber = newIcon;

    update();
}

MapPositionDialog::~MapPositionDialog() {
    delete mapImage;
}
