#include "RegionsListView.h"
#include <QVBoxLayout>

RegionListView::RegionListView(QWidget *parent) : QWidget{parent} {
    setupUI();
    connectSlots();
}

void RegionListView::setupUI() {
    btnNew = new QPushButton("New");
    btnDuplicate = new QPushButton("Duplicate");
    btnRename = new QPushButton("Rename");
    btnRemove = new QPushButton("Remove");

    QHBoxLayout* buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(btnNew);
    buttonLayout->addWidget(btnDuplicate);
    buttonLayout->addWidget(btnRename);
    buttonLayout->addStretch();
    buttonLayout->addWidget(btnRemove);

    lwRegions = new QListWidget;

    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addLayout(buttonLayout);
    mainLayout->addWidget(lwRegions);

    setLayout(mainLayout);

    connect(lwRegions, &QListWidget::currentRowChanged, this, &RegionListView::selectedRegionChanged);
}

void RegionListView::connectSlots() {
    connect(btnRemove, &QPushButton::clicked, this, &RegionListView::removeRegionSlot);
    connect(btnDuplicate, &QPushButton::clicked, this, &RegionListView::duplicateRegion);
}

void RegionListView::duplicateRegion() {
    emit regionDuplicated(lwRegions->currentRow());
}

void RegionListView::addRegionSlot() {
    Region region;
    newRegionDialog->setRegionData(region);

    QString regionName = region.editorName;

    if (region.editorName.isEmpty()) {
        regionName = "Region #" + QString::number(regions->size() + 1);
    }

    lwRegions->addItem(regionName);

    emit regionAdded(region);
}

void RegionListView::removeRegionSlot() {
    lwRegions->removeItemWidget(lwRegions->currentItem());

    emit regionRemoved(lwRegions->currentRow());
}

void RegionListView::regionsLoaded(const std::vector<Region>* regionList) {
    regions = regionList;

    lwRegions->clear();

    for (const Region& region : *regionList) {
        lwRegions->addItem(region.editorName);
    }
}

void RegionListView::setNewRegionDialog(NewRegionDialog* newDialog) {
    // TODO Disconnect signals?

    newRegionDialog = newDialog;

    connect(btnNew, &QPushButton::clicked, newRegionDialog, &NewRegionDialog::show);
    connect(newRegionDialog, &NewRegionDialog::accepted, this, &RegionListView::addRegionSlot);
}
