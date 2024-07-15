#include "Tool.h"

Tool::Tool() {}

void Tool::setMousePosition(const QPoint& position) {
    mousePositionView = position;

    if (mousePositionView.x() < 0) mousePositionView.setX(0);
    if (mousePositionView.y() < 0) mousePositionView.setY(0);

    int regionWidth = regionList->at(selectedRegion).width * 32;
    int regionHeight = regionList->at(selectedRegion).height * 32;
    if (mousePositionView.x() > regionWidth) mousePositionView.setX(regionWidth);
    if (mousePositionView.y() > regionHeight) mousePositionView.setY(regionHeight);

    // TODO [CLEAN UP, PERFORMANCE?] Might make sense to align the mouse position to the grid here, otherwise it'll have to be done in all tool classes anyway
    mousePositionMap.setX(position.x() + viewX);
    mousePositionMap.setY(position.y() + viewY);
}

void Tool::setDragging(bool isDragging) {
    dragging = isDragging;
}

bool Tool::isDragging() {
    return dragging;
}

const QPoint& Tool::getMousePositionMap() {
    return mousePositionMap;
}

const QPoint& Tool::getMousePositionView() {
    return mousePositionView;
}

void Tool::setSelectedSprite(int spriteNumber) {
    selectedSprites.clear();
    selectedSprites.push_back(spriteNumber);

    spriteSelectionWidth = 1;
    spriteSelectionHeight = 1;
}

void Tool::setSelectedTiles(const std::vector<int>& newSelection, int sw, int sh) {
    selectedTiles.clear();
    selectedTiles = newSelection;

    tileSelectionWidth = sw;
    tileSelectionHeight = sh;
}

void Tool::setSelectedSprites(const std::vector<int>& newSelection, int sw, int sh) {
    selectedSprites.clear();
    selectedSprites = newSelection;

    spriteSelectionWidth = sw;
    spriteSelectionHeight = sh;
}

void Tool::setImageCache(ImageCache& cache) {
    imageCache = &cache;
}

void Tool::setRegionList(const std::vector<Region>* newRegionList) {
    regionList = newRegionList;
}

void Tool::setSelectedRegion(int newSelectedRegion) {
    selectedRegion = newSelectedRegion;
}

void Tool::setSpriteList(const std::vector<PK2::SpriteBase>* newSpriteList) {
    spriteList = newSpriteList;
}

bool Tool::isRegionListValid() {
    return regionList != nullptr;
}

const std::vector<PK2::SpriteBase>* Tool::getSpriteList() {
    return spriteList;
}

const Region& Tool::getSelectedRegion() {
    return regionList->at(selectedRegion);
}

void Tool::setMode(MODE newMode) {
    mode = newMode;
}

Tool::MODE Tool::getMode() {
    return mode;
}

void Tool::setViewX(int newX) {
    viewX = newX;
}

void Tool::setViewY(int newY) {
    viewY = newY;
}

int Tool::getViewX() {
    return viewX;
}

int Tool::getViewY() {
    return viewY;
}
