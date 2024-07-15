#pragma once

#include "PK2MapBaseMetadata.h"

#include <qwidget.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qcombobox.h>
#include <qpushbutton.h>
#include <qspinbox.h>

#include <vector>

#include "Profile.h"
#include "MapPositionDialog.h"

class MapMetadataView : public QWidget {
public:
	// pk2filePath should contain the path to the pk2stuff.bmp file
	MapMetadataView(QWidget* parent, const QString& pk2filePath);
	~MapMetadataView();

    void setMapPositionDialog(MapPositionDialog* newMapPositionDialog);

public slots:
	void mapMetadataChanged(const PK2::MapBaseMetadata* metadata);
    void profileLoaded(const Profile* newProfile);

    void saveMapMetadata(PK2::MapBaseMetadata& metadata);

private:
	void setupUI();
    void connectSlots();

	void loadMapIcons(const QString& pk2stuffPath);

private:
	QLineEdit* leName = nullptr;
	QLineEdit* leAuthor = nullptr;

	QSpinBox* sbLevelNr = nullptr;
	QSpinBox* sbTime = nullptr;

	QComboBox* cbIcon = nullptr;

	QSpinBox* sbMapX = nullptr;
	QSpinBox* sbMapY = nullptr;

	QPushButton* btnSetMapPosition = nullptr;

    MapPositionDialog* mapPositionDialog;

	std::vector<QIcon> mapIcons;
};

