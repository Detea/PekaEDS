#include <QApplication>

#include "PekaEDSWindow.h"

int main(int argc, char* argv[]) {
	QApplication app(argc, argv);

	PekaEDSWindow mainWindow;
	mainWindow.show();

	app.exec();
}
