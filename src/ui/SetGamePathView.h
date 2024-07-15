#ifndef SETGAMEPATHVIEW_H
#define SETGAMEPATHVIEW_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>

class SetGamePathView : public QWidget {
    Q_OBJECT

public:
    explicit SetGamePathView(QWidget *parent = nullptr);

signals:
    void pathSet(const QString& newPath);

private slots:
    void showFileDialog();
    void done();

private:
    void setupUI();

private:
    QLineEdit* lePath = nullptr;
    QPushButton* btnBrowse = nullptr;
    QPushButton* btnOk = nullptr;
};

#endif // SETGAMEPATHVIEW_H
