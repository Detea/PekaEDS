#include "NewRegionDialog.h"
#include <QBoxLayout>

NewRegionDialog::NewRegionDialog(QWidget* parent) : QDialog(parent) {
    setupUI();
    connectSlots();
}

void NewRegionDialog::setupUI() {
    btnOk = new QPushButton("OK");
    btnCancel = new QPushButton("Cancel");

    QHBoxLayout* buttonLayout = new QHBoxLayout;
    buttonLayout->addStretch();
    buttonLayout->addWidget(btnOk);
    buttonLayout->addWidget(btnCancel);

    view = new RegionMetadataView(this);

    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addWidget(view);
    mainLayout->addLayout(buttonLayout);

    setLayout(mainLayout);

    btnOk->setDefault(true);

    setWindowTitle("Create a new region...");
}

void NewRegionDialog::connectSlots() {
    connect(this, &NewRegionDialog::profileLoaded, view, &RegionMetadataView::profileLoaded);

    connect(btnOk, &QPushButton::clicked, this, &NewRegionDialog::accept);
    connect(btnCancel, &QPushButton::clicked, this, &NewRegionDialog::reject);
}

void NewRegionDialog::setRegionData(Region& targetRegion) {
    view->setRegionData(targetRegion);
}
