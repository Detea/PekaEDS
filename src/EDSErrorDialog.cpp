#include "EDSErrorDialog.h"
#include <qboxlayout.h>

EDSErrorDialog::EDSErrorDialog(QWidget* parent) : QDialog(parent) {
	setupUI();
}

void EDSErrorDialog::show(const std::vector<QString>* errorMessages) {
	textEdit->clear();

	QString text;
	for (const QString& message : *errorMessages) {
		text += message;
		text += "\n";
	}

	textEdit->setText(text);

	setWindowTitle("Error(s) occurred while trying to load sprites...");

	setVisible(true);
}

void EDSErrorDialog::setupUI() {
	textEdit = new QTextEdit(this);

	textEdit->setReadOnly(true);

	QHBoxLayout* hbox = new QHBoxLayout;
	hbox->addWidget(textEdit);

	setLayout(hbox);

	setFixedSize(450, 300);
	setWindowTitle("Error(s) occurred while loading the map...");
}