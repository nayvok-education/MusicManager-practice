#include "deletewindow.h"
#include "ui_deletewindow.h"

deleteWindow::deleteWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::deleteWindow)
{
    ui->setupUi(this);

    // Создание модели данных для QListView
    QStringListModel* model = createListModel();

    // Установка модели данных в QListView
    ui->listView->setModel(model);
}

deleteWindow::~deleteWindow()
{
    delete ui;
}

// Создание модели данных для QListView
QStringListModel* deleteWindow::createListModel()
{
    // Загрузка JSON-файла
    QFile file("C:/ProjectsQT/MusicManager/DataBase/MusicDB.json");
    if (!file.open(QIODevice::ReadOnly))
    {
        // Обработка ошибки открытия файла
        return nullptr;
    }

    // Чтение JSON-данных из файла
    QByteArray jsonData = file.readAll();
    file.close();

    // Преобразование JSON-данных в документ
    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData);
    if (!jsonDoc.isObject())
    {
        // Обработка ошибки преобразования JSON-данных
        return nullptr;
    }

    // Получение корневого объекта JSON
    QJsonObject rootObj = jsonDoc.object();

    // Получение массива альбомов
    QJsonArray albumsArray = rootObj.value("albums").toArray();

    // Создание QStringList для хранения названий альбомов
    QStringList albumNames;

    // Добавление названий альбомов в QStringList
    for (const QJsonValueRef albumValue : albumsArray)
    {
        QJsonObject albumObj = albumValue.toObject();
        QString albumName = albumObj.value("name").toString();
        albumNames.append(albumName);
    }

    // Создание модели данных QStringListModel
    QStringListModel* model = new QStringListModel(albumNames);

    return model;
}

void deleteWindow::on_deleteButton_clicked()
{
    // Получение выбранного альбома из QListView
    QModelIndex selectedIndex = ui->listView->currentIndex();
    QString selectedAlbum = selectedIndex.data(Qt::DisplayRole).toString();

    // Загрузка JSON-файла
    QFile file("C:/ProjectsQT/MusicManager/DataBase/MusicDB.json");
    if (!file.open(QIODevice::ReadWrite))
    {
        // Обработка ошибки открытия файла
        QMessageBox::critical(this, "Ошибка", "Не удалось открыть файл JSON для записи");
        return;
    }

    // Чтение JSON-данных из файла
    QByteArray jsonData = file.readAll();

    // Преобразование JSON-данных в документ
    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData);
    if (!jsonDoc.isObject())
    {
        // Обработка ошибки преобразования JSON-данных
        QMessageBox::critical(this, "Ошибка", "Некорректный формат JSON-данных");
        return;
    }

    // Получение корневого объекта JSON
    QJsonObject rootObj = jsonDoc.object();

    // Получение массива альбомов
    QJsonArray albumsArray = rootObj.value("albums").toArray();

    // Поиск и удаление альбома по названию
    for (int i = 0; i < albumsArray.size(); ++i)
    {
        QJsonObject albumObj = albumsArray.at(i).toObject();
        if (albumObj.value("name").toString() == selectedAlbum)
        {
            albumsArray.removeAt(i);
            break;
        }
    }

    // Обновление корневого объекта JSON
    rootObj["albums"] = albumsArray;

    // Преобразование обновленного JSON-документа в данные
    QByteArray updatedData = QJsonDocument(rootObj).toJson();

    // Перемещение указателя файла в начало
    file.seek(0);

    // Запись обновленных данных в файл
    file.write(updatedData);

    // Усечение файла до размера записанных данных
    file.resize(updatedData.size());

    // Закрытие файла
    file.close();

    // Обновление модели данных в QListView
    QStringListModel* model = createListModel();
    ui->listView->setModel(model);

    QMessageBox::information(this, "Успех", "Альбом успешно удален");
}
