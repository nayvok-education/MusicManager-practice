#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QJsonObject>
#include <QJsonParseError>
#include <QFile>
#include <QStandardItem>
#include <QJsonArray>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    QJsonDocument doc;
    QJsonParseError docError;
    QString globPath;
    QJsonArray docArr;

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_BarAddAlbum_triggered();

    void on_BarAbout_triggered();

    void on_refresh_clicked();

    void on_searchButton_clicked();

    void on_searchInput_textChanged();

    void on_BarDeleteAlbum_triggered();

private:
    Ui::MainWindow *ui;
    void loadAlbumData();
    QStandardItemModel* model;
};

#endif // MAINWINDOW_H
