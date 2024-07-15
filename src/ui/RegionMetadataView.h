#ifndef REGIONMETADATAVIEW_H
#define REGIONMETADATAVIEW_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QSpinBox>
#include <QComboBox>

#include "Region.h"
#include "Profile.h"
#include "GamePaths.h"
#include "ColorPreviewWidget.h"

/*
 * The ...View classes only display data. When they do change something they send a signal to MapHandler.
 *
 * This way all the changing of data will happen in one place only.
 */
class RegionMetadataView : public QWidget {
    Q_OBJECT
public:
    explicit RegionMetadataView(QWidget *parent = nullptr);

    // Sets the data in region to the data entered in this widget
    void setRegionData(Region& region);

    static void setGamePaths(const GamePaths* newGamePaths);

public slots:
    void selectedRegionChanged(const Region& region);
    void profileLoaded(const Profile* newProfile);

private slots:
    void browseTileset();
    void browseTilesetBg();
    void browseBackground();
    void browseMusic();

    void clearBgTileset();

signals:
    void tilesetChanged(const QString& newTileset);
    void tilesetBackgroundChanged(const QString& newTileset);
    void backgroundChanged(const QString& newBackground);

    void backgroundTilesetCleared();

private:
    void setupUI();
    void connectSlots();

private:
    QLineEdit* leEditorName = nullptr;
    QLineEdit* leTilesetFile = nullptr;
    QLineEdit* leTilesetBgFile = nullptr;
    QLineEdit* leBackgroundFile = nullptr;
    QLineEdit* leMusicFile = nullptr;

    QPushButton* btnBrowseTileset = nullptr;
    QPushButton* btnBrowseTilesetBg = nullptr;
    QPushButton* btnClearTilesetBg = nullptr;
    QPushButton* btnBrowseBackground = nullptr;
    QPushButton* btnBrowseMusic = nullptr;

    QSpinBox* spWidth = nullptr;
    QSpinBox* spHeight = nullptr;

    QSpinBox* spFireColor1 = nullptr;
    QSpinBox* spFireColor2 = nullptr;
    QSpinBox* spSplashColor = nullptr;

    QComboBox* cbWeather = nullptr;
    QComboBox* cbScrolling = nullptr;

    ColorPreviewWidget* fireColor1Preview = nullptr;
    ColorPreviewWidget* fireColor2Preview = nullptr;
    ColorPreviewWidget* splashColorPreview = nullptr;

    // This is static because RegionMetadataView gets reused in NewRegionDialog and gamePaths doesn't change after being loaded.
    inline static const GamePaths* gamePaths = nullptr;
};

#endif // REGIONMETADATAVIEW_H
