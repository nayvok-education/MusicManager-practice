#ifndef DELETEWINDOW_H
#define DELETEWINDOW_H
#include <QStringListModel>
#include <QListView>
#include <QDialog>
#include <QFile>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QMessageBox>

namespace Ui {
class deleteWindow;
}

class deleteWindow : public QDialog
{
    Q_OBJECT

public:
    explicit deleteWindow(QWidget *parent = nullptr);
    ~deleteWindow();

private slots:
    void on_deleteButton_clicked();

private:
    Ui::deleteWindow *ui;
    QStringListModel* createListModel();
};

#endif // DELETEWINDOW_H
