#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "album.h"
#include "welcomewindow.h"
#include "addwindow.h"
#include "deletewindow.h"
#include "infowindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);

    loadAlbumData();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_BarAddAlbum_triggered()
{
    AddWindow addwin;
    addwin.setModal(true);
    addwin.exec();
}

void MainWindow::on_BarAbout_triggered()
{
    InfoWindow infowin;
    infowin.setModal(true);
    infowin.exec();
}

void MainWindow::on_BarDeleteAlbum_triggered()
{
    deleteWindow deletewin;
    deletewin.setModal(true);
    deletewin.exec();
}

void MainWindow::on_refresh_clicked()
{
    loadAlbumData();
}

void MainWindow::loadAlbumData()
{
    QFile file("C:/ProjectsQT/MusicManager/DataBase/MusicDB.json");

    if (file.open(QIODevice::ReadOnly | QFile::Text))
    {
        doc = QJsonDocument::fromJson(QByteArray(file.readAll()));
    }
    file.close();

    model = new QStandardItemModel(nullptr);
    model->setHorizontalHeaderLabels(QStringList() << "cover_photo" << "name" << "songs" << "artist" << "year" << "country");

    docArr = QJsonValue(doc.object().value("albums")).toArray();
    for (int i = 0; i < docArr.count(); i++)
    {
        QJsonObject albumObj = docArr.at(i).toObject();

        // Создаем объект Album
        std::string albumName = albumObj.value("name").toString().toStdString();
        std::string coverPhoto = albumObj.value("cover_photo").toString().toStdString();
        std::string artist = albumObj.value("artist").toString().toStdString();
        int year = albumObj.value("year").toInt();
        std::string country = albumObj.value("country").toString().toStdString();

        // Создаем список объектов Song из массива "songs" в JSON-файле
        std::list<Song> songs;
        QJsonArray songsArr = albumObj.value("songs").toArray();
        for (int j = 0; j < songsArr.count(); j++)
        {
            QString title = songsArr.at(j).toString();
            Song song(title.toStdString(), ""); // Предполагается, что в JSON нет информации об исполнителе
            songs.push_back(song);
        }

        Album album(albumName, coverPhoto, artist, songs, year, country);

        // Используем геттеры класса Album для получения информации об альбоме
        QStandardItem* item_col_1 = new QStandardItem();
        item_col_1->setIcon(QIcon(QString::fromStdString(album.getCoverPhoto())));

        QStandardItem* item_col_2 = new QStandardItem(QString::fromStdString(album.getAlbumName()));

        // Преобразуем список объектов Song в строку
        QString songsStr = "";
        unsigned int songCount = songs.size();
        unsigned int currentSong = 0;
        for (const Song& song : songs)
        {
            songsStr += QString::fromStdString(song.getTitle());
            if (currentSong < songCount - 1)
            {
                songsStr += "\n";
            }
            currentSong++;
        }

        QStandardItem* item_col_3 = new QStandardItem(songsStr);
        QStandardItem* item_col_4 = new QStandardItem(QString::fromStdString(album.getArtist()));
        QStandardItem* item_col_5 = new QStandardItem(QString::number(album.getYear()));
        QStandardItem* item_col_6 = new QStandardItem(QString::fromStdString(album.getCountry()));

        model->appendRow(QList<QStandardItem*>() << item_col_1 << item_col_2 << item_col_3 << item_col_4 << item_col_5 << item_col_6);
    }
    ui->albumTable->setModel(model);
    ui->albumTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->albumTable->resizeRowsToContents();
    ui->albumTable->setIconSize(QSize(100, 100));
    ui->albumTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->albumTable->setSelectionMode(QAbstractItemView::NoSelection);
    ui->albumTable->setStyleSheet("QTableView {"
                                   "background-color: white;"
                                   "color:black;"
                                   "border-style: none;"
                                   "border-top: 1px solid black;"
                                   "border-bottom: 1px solid black;"
                                   "}"
                                   "QTableView::item:selected {"
                                   "background-color: white;"
                                   "color:black"
                                   "}"
                                   "QTableView::item:hover {"
                                   "background-color: white;"
                                   "color:black"
                                   "}"
                                   "QTableView::item {"
                                   "background-color: white;"
                                   "border-bottom: 1px solid black;"
                                   "color:black"
                                   "}");

    // Проверяем, если поле поиска не пустое, выполняем поиск
    QString searchText = ui->searchInput->toPlainText();
    if (!searchText.isEmpty())
    {
        on_searchButton_clicked();
    }
}

void MainWindow::on_searchButton_clicked()
{
    QString searchType = ui->filters->currentText();
    QString searchText = ui->searchInput->toPlainText();

    for (int row = 0; row < model->rowCount(); row++)
    {
        QModelIndex index;
        bool match = false;

        if (searchType == "По названию альбома")
        {
            index = model->index(row, 1); // Индекс столбца с названием альбома
            QString albumName = model->data(index).toString();
            match = albumName.contains(searchText, Qt::CaseInsensitive);
        }
        else if (searchType == "По названию песни")
        {
            index = model->index(row, 2); // Индекс столбца с названием песни
            QString songName = model->data(index).toString();
            match = songName.contains(searchText, Qt::CaseInsensitive);
        }
        else if (searchType == "По исполнителю")
        {
            index = model->index(row, 3); // Индекс столбца с названием песни
            QString songName = model->data(index).toString();
            match = songName.contains(searchText, Qt::CaseInsensitive);
        }
        else if (searchType == "По дате")
        {
            QIntValidator validator;
            int pos = 0;
            QValidator::State state = validator.validate(searchText, pos);
            if (state == QValidator::Acceptable && searchText == QString::number(searchText.toInt()))
            {
                index = model->index(row, 4); // Индекс столбца с годом
                int albumYear = model->data(index).toInt();
                match = (albumYear == searchText.toInt());
            }
            else
            {
                QMessageBox::warning(this, "Некорректный ввод", "Пожалуйста, введите только цифры для фильтра 'По дате'.");
                return; // Возвращаемся, чтобы предотвратить скрытие строк и выполнение поиска
            }
        }
        else if (searchType == "По стране")
        {
            QRegExpValidator validator(QRegExp("[А-Яа-я]*")); // Валидатор для букв
            int pos = 0;
            QValidator::State state = validator.validate(searchText, pos);
            if (state == QValidator::Acceptable)
            {
                index = model->index(row, 5); // Индекс столбца со страной
                QString country = model->data(index).toString();
                match = country.contains(searchText, Qt::CaseInsensitive);
            }
            else
            {
                QMessageBox::warning(this, "Некорректный ввод", "Пожалуйста, введите только буквы для фильтра 'По стране'.");
                return; // Возвращаемся, чтобы предотвратить скрытие строк и выполнение поиска
            }
        }

        ui->albumTable->setRowHidden(row, !match);
    }
}

void MainWindow::on_searchInput_textChanged()
{
    QString searchText = ui->searchInput->toPlainText();

    if (searchText.isEmpty())
    {
        // Если текст пустой, отображаем все альбомы
        for (int row = 0; row < model->rowCount(); row++)
        {
            ui->albumTable->setRowHidden(row, false);
        }
    }
}



