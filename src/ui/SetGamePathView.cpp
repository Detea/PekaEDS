#include "SetGamePathView.h"
#include <QBoxLayout>
#include <QLabel>
#include <QFileDialog>
#include <QDir>
#include <QMessageBox>

SetGamePathView::SetGamePathView(QWidget *parent) : QWidget(parent) {
    setupUI();
}

void SetGamePathView::setupUI() {
    QLabel* lblGamePath = new QLabel("Pekka Kana 2 folder:");
    lePath = new QLineEdit;
    btnBrowse = new QPushButton("Browse");
    btnOk = new QPushButton("OK");

    connect(btnBrowse, &QPushButton::clicked, this, &SetGamePathView::showFileDialog);
    connect(btnOk, &QPushButton::clicked, this, &SetGamePathView::done);

    lePath->setFixedWidth(700);

    QHBoxLayout* btnLayout = new QHBoxLayout;
    btnLayout->addStretch();
    btnLayout->addWidget(btnBrowse);
    btnLayout->addWidget(btnOk);

    QVBoxLayout* layout = new QVBoxLayout;
    layout->addStretch();
    layout->addWidget(lblGamePath);
    layout->addWidget(lePath);
    layout->addLayout(btnLayout);
    layout->addStretch();

    QHBoxLayout* centerLayout = new QHBoxLayout;
    centerLayout->addStretch();
    centerLayout->addLayout(layout);
    centerLayout->addStretch();

    setLayout(centerLayout);
}

void SetGamePathView::showFileDialog() {
    QString filePath =  QFileDialog::getExistingDirectory(this, "Choose the Pekka Kana 2 game folder...",
                                                          "",
                                                          QFileDialog::ShowDirsOnly |
                                                          QFileDialog::DontResolveSymlinks);

    lePath->setText(filePath);
}

void SetGamePathView::done() {
    const QString& gamePath = lePath->text();

    if (!gamePath.isEmpty()) {
        if (QFile::exists(gamePath)) {
            emit pathSet(lePath->text());
        } else {
            QMessageBox::warning(this, "Directory not found", "The selected directory does not exist!");
        }
    } else {
        QMessageBox::warning(this, "No path set!", "You need to select the game folder!");
    }
}
