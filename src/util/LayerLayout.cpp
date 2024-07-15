#include "LayerLayout.h"

LayerLayout::LayerLayout() {}

LayerLayout::LayerLayout(QWidget* parent) : QLayout(parent) {}

void LayerLayout::setGeometry(const QRect& rect) {
	QLayout::setGeometry(rect);

	if (items.size() == 0) {
		return;
	}

	for (int i = 0; i < items.size(); ++i) {
		QLayoutItem* item = items.at(i);

		QRect geoRect(rect.x(), rect.y(), rect.width(), rect.height());

		item->setGeometry(geoRect);
	}
}

int LayerLayout::count() const {
	return items.count();
}

QSize LayerLayout::sizeHint() const {
	QSize size(0, 0);

	if (!items.isEmpty()) {
		size = size.expandedTo(items.at(0)->sizeHint());
	}

	return size;
}

QSize LayerLayout::minimumSize() const {
	QSize size(0, 0);

	if (!items.isEmpty()) {
		size = size.expandedTo(items.at(0)->minimumSize());
	}

	return size;
}

QLayoutItem* LayerLayout::itemAt(int index) const {
	return items.value(index);
}

QLayoutItem* LayerLayout::takeAt(int index) {
	return index >= 0 && index < items.size() ? items.takeAt(index) : 0;
}

void LayerLayout::addItem(QLayoutItem* item) {
	items.append(item);
}

LayerLayout::~LayerLayout() {
	QLayoutItem* item;

	while ((item = takeAt(0))) {
		delete item;
	}
}