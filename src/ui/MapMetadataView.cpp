#include "MapMetadataView.h"
#include <qlabel.h>
#include <qgridlayout.h>
#include <qpixmap.h>
#include <qimage.h>

#include "GFXUtil.h"

MapMetadataView::MapMetadataView(QWidget* parent, const QString& pk2stuffPath) : QWidget(parent) {
	setupUI();

	loadMapIcons(pk2stuffPath);
}

MapMetadataView::~MapMetadataView() {

}

// TODO [CLEAN UP] Pass a reference, instead of a pointer
void MapMetadataView::profileLoaded(const Profile* newProfile) {
    if (mapIcons.size() == newProfile->iconNames.size()) {
        for (int i = 0; i < newProfile->iconNames.size(); ++i) {
            cbIcon->addItem(mapIcons[i], newProfile->iconNames[i]);
        }
    } else {
        for (int i = 0; i < newProfile->iconNames.size(); ++i) {
            cbIcon->addItem(newProfile->iconNames[i]);
        }
    }

    cbIcon->setIconSize(QSize(27, 27));
}

void MapMetadataView::saveMapMetadata(PK2::MapBaseMetadata& otherMetadata) {
    otherMetadata.name = leName->text();
    otherMetadata.author = leAuthor->text();
    otherMetadata.levelNumber = sbLevelNr->value();
    otherMetadata.time = sbTime->value();
    otherMetadata.iconNumber = cbIcon->currentIndex();
    otherMetadata.mapX = sbMapX->value();
    otherMetadata.mapY = sbMapY->value();
}

void MapMetadataView::setupUI() {
	QLabel* lblName = new QLabel("Map name:");
	leName = new QLineEdit;

	QLabel* lblAuthor = new QLabel("Author:");
	leAuthor = new QLineEdit;

    QLabel* lblLevelNr = new QLabel("Level number.:");
	sbLevelNr = new QSpinBox;

    QLabel* lblTime = new QLabel("Time (seconds):");
	sbTime = new QSpinBox;

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

	sbLevelNr->setFixedWidth(60);
	sbTime->setFixedWidth(60);

	sbMapX->setFixedWidth(60);
	sbMapY->setFixedWidth(60);

	sbLevelNr->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	sbTime->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

	sbMapX->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	sbMapY->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

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
	layout->addWidget(lblLevelNr, 5, 0);
	layout->addWidget(sbLevelNr, 5, 1);

	layout->addWidget(lblTime, 6, 0);
	layout->addWidget(sbTime, 6, 1);

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

void MapMetadataView::connectSlots() {
    connect(btnSetMapPosition, &QPushButton::clicked, mapPositionDialog, &MapPositionDialog::show);
    connect(sbMapX, &QSpinBox::valueChanged, mapPositionDialog, &MapPositionDialog::updateX);
    connect(sbMapY, &QSpinBox::valueChanged, mapPositionDialog, &MapPositionDialog::updateY);
    connect(cbIcon, &QComboBox::currentIndexChanged, mapPositionDialog, &MapPositionDialog::updateIconNumber);

    connect(mapPositionDialog, &MapPositionDialog::positionXChanged, sbMapX, &QSpinBox::setValue);
    connect(mapPositionDialog, &MapPositionDialog::positionYChanged, sbMapY, &QSpinBox::setValue);
}

void MapMetadataView::mapMetadataChanged(const PK2::MapBaseMetadata* metadata) {
    leName->setText(metadata->name);
	leAuthor->setText(metadata->author);

    sbLevelNr->setValue(metadata->levelNumber);
	sbTime->setValue(metadata->time);
	cbIcon->setCurrentIndex(metadata->iconNumber);

	sbMapX->setValue(metadata->mapX);
	sbMapY->setValue(metadata->mapY);
}

void MapMetadataView::loadMapIcons(const QString& pk2stuffPath) {
	// TODO What to do when the file can't be found?
	QImage img;

	if (img.load(pk2stuffPath)) {
		GFXUtil::makeTransparent(&img);

		QPixmap pixmap;

		if (pixmap.convertFromImage(img)) {
			constexpr int iconSize = 27;
			int icons = (pixmap.width() / iconSize) - 1; // Subtracting 1 because the mouse cursor icon is stored after the map icons

			QPixmap p;
			for (int i = 0; i < icons; ++i) {
				p = pixmap.copy(1 + (i * 28), 452, 27, 27);

				mapIcons.push_back(QIcon(p));
			}
		} else {
			// TODO Show an error? Fail silently?
		}
	} else {
		qDebug() << "Unable to load pk2stuff file. Provided path: " << pk2stuffPath;
	}
}

void MapMetadataView::setMapPositionDialog(MapPositionDialog* newMapPositionDialog) {
    mapPositionDialog = newMapPositionDialog;

    connectSlots();

}
