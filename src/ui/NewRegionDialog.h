#ifndef NEWREGIONDIALOG_H
#define NEWREGIONDIALOG_H

#include <QDialog>
#include <QPushButton>
#include "Region.h"
#include "RegionMetadataView.h"

class NewRegionDialog : public QDialog {
    Q_OBJECT

public:
    NewRegionDialog(QWidget* parent);

    void setRegionData(Region& targetRegion);
    void setGamePaths(const GamePaths* newGamePaths);

signals:
    void profileLoaded(const Profile* newProfile);

private:
    void setupUI();
    void connectSlots();

private:
    QPushButton* btnOk = nullptr;
    QPushButton* btnCancel = nullptr;

    RegionMetadataView* view = nullptr;
    Region* region = nullptr;
};

#endif // NEWREGIONDIALOG_H
