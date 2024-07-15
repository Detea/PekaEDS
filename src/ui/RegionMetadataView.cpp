#include "RegionMetadataView.h"
#include <QLabel>
#include <QBoxLayout>
#include <QGridLayout>
#include <QFileDialog>

RegionMetadataView::RegionMetadataView(QWidget *parent) : QWidget{parent} {
    setupUI();
    connectSlots();
}

void RegionMetadataView::setupUI() {
    leEditorName = new QLineEdit;
    leTilesetFile = new QLineEdit;
    leTilesetBgFile = new QLineEdit;
    leBackgroundFile = new QLineEdit;
    leMusicFile = new QLineEdit;

    spWidth = new QSpinBox;
    spHeight = new QSpinBox;

    spWidth->setRange(1, 65536);
    spHeight->setRange(1, 65536);

    spWidth->setValue(100);
    spHeight->setValue(100);

    spFireColor1 = new QSpinBox;
    spFireColor2 = new QSpinBox;
    spSplashColor = new QSpinBox;

    spFireColor1->setRange(0, 255);
    spFireColor2->setRange(0, 255);
    spSplashColor->setRange(-1, 255);

    spFireColor1->setValue(64);
    spFireColor2->setValue(128);
    spSplashColor->setValue(-1);

    spFireColor1->setSingleStep(32);
    spFireColor2->setSingleStep(32);
    spSplashColor->setSingleStep(32);

    cbWeather = new QComboBox;
    cbScrolling = new QComboBox;

    QLabel* lblEditorName = new QLabel("Name:");
    QLabel* lblTileset = new QLabel("Tileset:");
    QLabel* lblTilesetBg = new QLabel("Tileset BG:");
    QLabel* lblBackground = new QLabel("Background:");
    QLabel* lblMusic = new QLabel("Music:");

    QLabel* lblWidth = new QLabel("Width:");
    QLabel* lblHeight = new QLabel("Height:");

    QLabel* lblWeather = new QLabel("Weather:");
    QLabel* lblScrolling = new QLabel("Scrolling:");

    QLabel* lblFireColor1 = new QLabel("Fire Color 1:");
    QLabel* lblFireColor2 = new QLabel("Fire Color 2:");
    QLabel* lblSplashColor = new QLabel("Splash Color:");

    btnBrowseTileset = new QPushButton("Browse");
    btnBrowseTilesetBg = new QPushButton("Browse");
    btnClearTilesetBg = new QPushButton("Clear");
    btnBrowseBackground = new QPushButton("Browse");
    btnBrowseMusic = new QPushButton("Browse");

    fireColor1Preview = new ColorPreviewWidget(this);
    fireColor2Preview = new ColorPreviewWidget(this);
    splashColorPreview = new ColorPreviewWidget(this);

    QGridLayout* mainLayout = new QGridLayout;
    mainLayout->addWidget(lblEditorName, 0, 0);
    mainLayout->addWidget(leEditorName, 0, 1);

    mainLayout->addWidget(lblTileset, 1, 0);
    mainLayout->addWidget(leTilesetFile, 1, 1);
    mainLayout->addWidget(btnBrowseTileset, 1, 2);

    mainLayout->addWidget(lblTilesetBg, 2, 0);
    mainLayout->addWidget(leTilesetBgFile, 2, 1);
    mainLayout->addWidget(btnBrowseTilesetBg, 2, 2);
    mainLayout->addWidget(btnClearTilesetBg, 2, 3);

    mainLayout->addWidget(lblBackground, 3, 0);
    mainLayout->addWidget(leBackgroundFile, 3, 1);
    mainLayout->addWidget(btnBrowseBackground, 3, 2);

    mainLayout->addWidget(lblMusic, 4, 0);
    mainLayout->addWidget(leMusicFile, 4, 1);
    mainLayout->addWidget(btnBrowseMusic, 4, 2);

    mainLayout->addWidget(lblWidth, 5, 0);
    mainLayout->addWidget(spWidth, 5, 1);

    mainLayout->addWidget(lblHeight, 6, 0);
    mainLayout->addWidget(spHeight, 6, 1);

    mainLayout->addWidget(lblWeather, 7, 0);
    mainLayout->addWidget(cbWeather, 7, 1);

    mainLayout->addWidget(lblScrolling, 8, 0);
    mainLayout->addWidget(cbScrolling, 8, 1);

    mainLayout->addWidget(lblFireColor1, 9, 0);
    mainLayout->addWidget(spFireColor1, 9, 1);
    mainLayout->addWidget(fireColor1Preview, 9, 2);

    mainLayout->addWidget(lblFireColor2, 10, 0);
    mainLayout->addWidget(spFireColor2, 10, 1);
    mainLayout->addWidget(fireColor2Preview, 10, 2);

    mainLayout->addWidget(lblSplashColor, 11, 0);
    mainLayout->addWidget(spSplashColor, 11, 1);
    mainLayout->addWidget(splashColorPreview, 11, 2);

    QVBoxLayout* layout = new QVBoxLayout;
    layout->addLayout(mainLayout);
    layout->addStretch();

    setLayout(layout);
}

void RegionMetadataView::connectSlots() {
    connect(btnBrowseTileset, &QPushButton::clicked, this, &RegionMetadataView::browseTileset);
    connect(btnBrowseTilesetBg, &QPushButton::clicked, this, &RegionMetadataView::browseTilesetBg);
    connect(btnClearTilesetBg, &QPushButton::clicked, this, &RegionMetadataView::clearBgTileset);

    connect(btnBrowseBackground, &QPushButton::clicked, this, &RegionMetadataView::browseBackground);
    connect(btnBrowseMusic, &QPushButton::clicked, this, &RegionMetadataView::browseMusic);

    connect(spFireColor1, &QSpinBox::valueChanged, fireColor1Preview, &ColorPreviewWidget::setSelectedColor);
    connect(spFireColor2, &QSpinBox::valueChanged, fireColor2Preview, &ColorPreviewWidget::setSelectedColor);
    connect(spSplashColor, &QSpinBox::valueChanged, splashColorPreview, &ColorPreviewWidget::setSelectedColor);

    connect(fireColor1Preview, &ColorPreviewWidget::selectedColorChanged, spFireColor1, &QSpinBox::setValue);
    connect(fireColor2Preview, &ColorPreviewWidget::selectedColorChanged, spFireColor2, &QSpinBox::setValue);
    connect(splashColorPreview, &ColorPreviewWidget::selectedColorChanged, spSplashColor, &QSpinBox::setValue);
}

void RegionMetadataView::selectedRegionChanged(const Region& region) {
    leEditorName->setText(region.editorName);
    leTilesetFile->setText(region.tilesetImageFile);
    leTilesetBgFile->setText(region.tilesetBackgroundImageFile);
    leBackgroundFile->setText(region.backgroundImageFile);
    leMusicFile->setText(region.musicFile);

    spWidth->setValue(region.width);
    spHeight->setValue(region.height);

    spFireColor1->setValue(region.fireColor1);
    spFireColor2->setValue(region.fireColor2);
    spSplashColor->setValue(region.splashColor);

    fireColor1Preview->setSelectedColor(region.fireColor1);
    fireColor2Preview->setSelectedColor(region.fireColor2);
    splashColorPreview->setSelectedColor(region.splashColor);

    cbWeather->setCurrentIndex(region.weather);
    cbScrolling->setCurrentIndex(region.scrolling);

    fireColor1Preview->selectedRegionChanged(region);
    fireColor2Preview->selectedRegionChanged(region);
    splashColorPreview->selectedRegionChanged(region);
}

void RegionMetadataView::setRegionData(Region& region) {
    region.editorName = leEditorName->text();
    region.tilesetImageFile = leTilesetFile->text();
    region.tilesetBackgroundImageFile = leTilesetBgFile->text();
    region.backgroundImageFile = leBackgroundFile->text();
    region.musicFile = leMusicFile->text();

    region.width = spWidth->value();
    region.height = spHeight->value();

    region.fireColor1 = spFireColor1->value();
    region.fireColor2 = spFireColor2->value();
    region.splashColor = spSplashColor->value();

    region.weather = cbWeather->currentIndex();
    region.scrolling = cbScrolling->currentIndex();
}

void RegionMetadataView::profileLoaded(const Profile* newProfile) {
    cbWeather->clear();

    for (const QString& str : newProfile->weatherTypes) {
        cbWeather->addItem(str);
    }

    cbScrolling->clear();

    for (const QString& str : newProfile->scrollingTypes) {
        cbScrolling->addItem(str);
    }

    // TODO [CLEAN UP] This shouldn't be done in here, do it at the setup and reset stage
    leTilesetFile->setText("tiles01.bmp");
    leBackgroundFile->setText("field3_d.bmp");
    leMusicFile->setText("song01.xm");
}

void RegionMetadataView::browseTileset() {
    // TODO Game/Episode path
    QString tileset = QFileDialog::getOpenFileName(this,
                                                   "Select a tileset image file...",
                                                   gamePaths->tilesetFolder(),
                                                   "256 color bitmap image (*.bmp *.BMP)");
    tileset = tileset.split("/").last();

    if (!tileset.isEmpty()) {
        leTilesetFile->setText(tileset);

        emit tilesetChanged(tileset);
    }
}

void RegionMetadataView::browseTilesetBg() {
    // TODO Game/Episode path
    QString tileset = QFileDialog::getOpenFileName(this,
                                                   "Select a tileset image file...",
                                                   gamePaths->tilesetFolder(),
                                                   "256 color bitmap image (*.bmp *.BMP)");
    tileset = tileset.split("/").last();

    if (!tileset.isEmpty()) {
        leTilesetBgFile->setText(tileset);

        emit tilesetBackgroundChanged(tileset);
    }
}

void RegionMetadataView::clearBgTileset() {
    leTilesetBgFile->setText("");

    emit backgroundTilesetCleared();
}

void RegionMetadataView::browseBackground() {
    // TODO Game/Episode path
    QString backgroundFile = QFileDialog::getOpenFileName(this,
                                                          "Select a background image file...",
                                                          gamePaths->backgroundFolder(),
                                                          "256 color bitmap image (*.bmp *.BMP)");
    backgroundFile = backgroundFile.split("/").last();

    if (!backgroundFile.isEmpty()) {
        leBackgroundFile->setText(backgroundFile);

        // TODO how to handle loading from episode path?
        emit backgroundChanged(backgroundFile);
    }
}

void RegionMetadataView::browseMusic() {
    // TODO Game/Episode path
    QString musicFile = QFileDialog::getOpenFileName(this,
                                                     "Select a music file...",
                                                     gamePaths->musicFolder(),
                                                     "Supported music file formats (*.xm *.XM *.mod *.MOD *.it *.IT *.s3m *.S3M)");
    musicFile = musicFile.split("/").last();

    if (!musicFile.isEmpty()) {
        leMusicFile->setText(musicFile);
    }
}

void RegionMetadataView::setGamePaths(const GamePaths* newGamePaths) {
    gamePaths = newGamePaths;
}
