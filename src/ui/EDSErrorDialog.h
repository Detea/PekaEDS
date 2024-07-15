#pragma once
#include <qdialog.h>
#include <qtextedit.h>

#include <vector>
#include <qstring.h>

class EDSErrorDialog : public QDialog {
public:
	EDSErrorDialog(QWidget* parent = nullptr);

public slots:
    void showErrors(const std::vector<QString>& errorMessages);

private:
	void setupUI();

private:
	QTextEdit* textEdit = nullptr;
};

