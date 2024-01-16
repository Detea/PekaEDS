#pragma once
#include "MapMetadataComponent.h"

#include <qwidget.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qcombobox.h>
#include <qpushbutton.h>
#include <qspinbox.h>

class MapMetadataView : public QWidget, public MapMetadataComponent {
public:
	MapMetadataView(QWidget* parent = nullptr);
	~MapMetadataView();

public slots:
	void mapMetadataChanged(const PK2::MapBaseMetadata* metadata);

private:
	void setupUI();

private:
	QLineEdit* leName = nullptr;
	QLineEdit* leAuthor = nullptr;
	QLineEdit* leTileset = nullptr;
	QLineEdit* leBackground = nullptr;
	QLineEdit* leMusic = nullptr;

	QPushButton* btnLoadTileset = nullptr;
	QPushButton* btnLoadBackground = nullptr;
	QPushButton* btnLoadMusic = nullptr;

	QSpinBox* sbLevelNr = nullptr;
	QSpinBox* sbTime = nullptr;

	QComboBox* cbWeather = nullptr;
	QComboBox* cbScrolling = nullptr;
	QComboBox* cbIcon = nullptr;

	QSpinBox* sbMapX = nullptr;
	QSpinBox* sbMapY = nullptr;

	QPushButton* btnSetMapPosition = nullptr;
};

