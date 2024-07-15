#ifndef REGIONSLISTVIEW_H
#define REGIONSLISTVIEW_H

#include <QWidget>
#include <QListWidget>
#include <QPushButton>

#include <vector>

#include "Region.h"
#include "NewRegionDialog.h"

class RegionListView : public QWidget {
    Q_OBJECT

public:
    explicit RegionListView(QWidget *parent = nullptr);

    void setNewRegionDialog(NewRegionDialog* newDialog);

public slots:
    void regionsLoaded(const std::vector<Region>* regionList);

signals:
    void selectedRegionChanged(int selectedRegionIndex);
    void regionAdded(const Region& addedRegion);
    void regionRemoved(int regionIndex);
    void regionDuplicated(int regionIndex);

private slots:
    void addRegionSlot();
    void removeRegionSlot();
    void duplicateRegion();

private:
    void setupUI();
    void connectSlots();

private:
    const std::vector<Region>* regions = nullptr;

    QListWidget* lwRegions = nullptr;

    QPushButton* btnNew = nullptr;
    QPushButton* btnDuplicate = nullptr;
    QPushButton* btnRename = nullptr;
    QPushButton* btnRemove = nullptr;

    NewRegionDialog* newRegionDialog = nullptr;
};

#endif // REGIONSLISTVIEW_H
