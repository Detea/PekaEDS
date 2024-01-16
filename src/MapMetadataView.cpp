#include "MapMetadataView.h"
#include <qlabel.h>
#include <qgridlayout.h>

MapMetadataView::MapMetadataView(QWidget* parent) : QWidget(parent) {
	setupUI();
}

MapMetadataView::~MapMetadataView() {

}

void MapMetadataView::setupUI() {
	QLabel* lblName = new QLabel("Map name:");
	leName = new QLineEdit;

	QLabel* lblAuthor = new QLabel("Author:");
	leAuthor = new QLineEdit;
	
	QLabel* lblTileset = new QLabel("Tileset:");
	leTileset = new QLineEdit;
	btnLoadTileset = new QPushButton("...");

	QLabel* lblBackground = new QLabel("Background:");
	leBackground = new QLineEdit;
	btnLoadBackground = new QPushButton("...");

	QLabel* lblMusic = new QLabel("Music:");
	leMusic = new QLineEdit;
	btnLoadMusic = new QPushButton("...");

	QLabel* lblLevelNr = new QLabel("Level Nr.:");
	sbLevelNr = new QSpinBox;

	QLabel* lblTime = new QLabel("Time (sec):");
	sbTime = new QSpinBox;

	QLabel* lblWeather = new QLabel("Weather:");
	cbWeather = new QComboBox;

	QLabel* lblScrolling = new QLabel("Scrolling:");
	cbScrolling = new QComboBox;

	QLabel* lblIcon = new QLabel("Icon:");
	cbIcon = new QComboBox;

	QLabel* lblMapX = new QLabel("Map X:");
	sbMapX = new QSpinBox;

	QLabel* lblMapY = new QLabel("Map Y:");
	sbMapY = new QSpinBox;

	btnSetMapPosition = new QPushButton("Set map position");

	sbLevelNr->setRange(0, 1000);
	sbTime->setRange(0, 10000);

	sbMapX->setRange(0, 10000);
	sbMapY->setRange(0, 10000);

	leName->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	leAuthor->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	leTileset->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	leBackground->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	leMusic->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

	sbLevelNr->setFixedWidth(60);
	sbTime->setFixedWidth(60);

	sbMapX->setFixedWidth(60);
	sbMapY->setFixedWidth(60);

	sbLevelNr->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	sbTime->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

	sbMapX->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	sbMapY->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

	btnLoadTileset->setFixedWidth(30);
	btnLoadBackground->setFixedWidth(30);
	btnLoadMusic->setFixedWidth(30);

	btnLoadTileset->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	btnLoadBackground->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	btnLoadMusic->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

	QVBoxLayout* vbox = new QVBoxLayout;
	QGridLayout* layout = new QGridLayout;
	layout->setSpacing(5);
	layout->setColumnStretch(0, 0);
	layout->setColumnStretch(1, 2);

	layout->addWidget(lblName, 0, 0);
	layout->addWidget(leName, 0, 1);
	
	layout->addWidget(lblAuthor, 1, 0);
	layout->addWidget(leAuthor, 1, 1);
	
	// TODO Add this?
	//layout->addItem(new QSpacerItem(1, 10), 2, 0);

	layout->addWidget(lblTileset, 2, 0);
	layout->addWidget(leTileset, 2, 1);
	layout->addWidget(btnLoadTileset, 2, 2);

	layout->addWidget(lblBackground, 3, 0);
	layout->addWidget(leBackground, 3, 1);
	layout->addWidget(btnLoadBackground, 3, 2);

	layout->addWidget(lblMusic, 4, 0);
	layout->addWidget(leMusic, 4, 1);
	layout->addWidget(btnLoadMusic, 4, 2);

	layout->addWidget(lblLevelNr, 5, 0);
	layout->addWidget(sbLevelNr, 5, 1);

	layout->addWidget(lblTime, 6, 0);
	layout->addWidget(sbTime, 6, 1);

	layout->addWidget(lblWeather, 7, 0);
	layout->addWidget(cbWeather, 7, 1);

	layout->addWidget(lblScrolling, 8, 0);
	layout->addWidget(cbScrolling, 8, 1);

	layout->addWidget(lblIcon, 9, 0);
	layout->addWidget(cbIcon, 9, 1);

	layout->addWidget(lblMapX, 10, 0);
	layout->addWidget(sbMapX, 10, 1);

	layout->addWidget(lblMapY, 11, 0);
	layout->addWidget(sbMapY, 11, 1);

	layout->addWidget(btnSetMapPosition, 12, 0);

	vbox->addLayout(layout);
	vbox->addStretch();

	setLayout(vbox);
}

void MapMetadataView::mapMetadataChanged(const PK2::MapBaseMetadata* metadata) {
	qDebug() << metadata->name;

	// TODO Use QString...
	leName->setText(QString::fromStdString(metadata->name));
	leAuthor->setText(QString::fromStdString(metadata->author));
	leTileset->setText(QString::fromStdString(metadata->tilesetFile));
	leBackground->setText(QString::fromStdString(metadata->backgroundFile));
}