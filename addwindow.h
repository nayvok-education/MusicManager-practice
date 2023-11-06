#ifndef ADDWINDOW_H
#define ADDWINDOW_H

#include <QDialog>
#include <QJsonObject>
#include <QJsonParseError>
#include <QFile>
#include <QStandardItem>
#include <QJsonArray>
#include <QMessageBox>
#include <QDir>
#include <QFileDialog>
#include <QFileInfo>


namespace Ui {
class AddWindow;
}

class AddWindow : public QDialog
{
    Q_OBJECT

    QJsonDocument doc;
    QJsonParseError docError;
    QString globPath;
    QJsonArray docArr;

public:
    explicit AddWindow(QWidget *parent = nullptr);
    ~AddWindow();

private slots:
    void on_AddButton_clicked();

    void on_CancelButton_clicked();

    void on_File_clicked();

private:
    Ui::AddWindow *ui;
    QString imagePathToAdd;
};

#endif // ADDWINDOW_H
