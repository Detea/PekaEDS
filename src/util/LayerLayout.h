#pragma once
#include <qlayout.h>
#include <qlayoutitem.h>
#include <qlist.h>

class LayerLayout : public QLayout {
public:
	LayerLayout();
	LayerLayout(QWidget* parent);
	~LayerLayout();

	void addItem(QLayoutItem* item) override;

	void setGeometry(const QRect& rect) override;

	int count() const;

	QSize sizeHint() const override;
	
	QSize minimumSize() const override;

	QLayoutItem* itemAt(int index) const override;
	QLayoutItem* takeAt(int index) override;

private:
	QList<QLayoutItem*> items;
};

