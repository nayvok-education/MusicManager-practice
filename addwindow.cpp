#include "addwindow.h"
#include "ui_addwindow.h"
#include <QDebug>

AddWindow::AddWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddWindow)
{
    ui->setupUi(this);
}

AddWindow::~AddWindow()
{
    delete ui;
}

void AddWindow::on_AddButton_clicked()
{
    if (ui->lineEdit->text().isEmpty() || ui->lineEdit_2->text().isEmpty() || ui->lineEdit_3->text().isEmpty() || ui->lineEdit_4->text().isEmpty() || ui->lineEdit_5->text().isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Пожалуйста, заполните все поля ввода.");
        return;
    }
    // Проверка на ввод букв или символов в lineEdit_3
    bool isLineEdit3Valid = false;
    ui->lineEdit_3->text().toInt(&isLineEdit3Valid);
    if (!isLineEdit3Valid) {
        QMessageBox::warning(this, "Ошибка", "Пожалуйста, введите корректный год в числовом формате.");
        return;
    }

    // Проверка на ввод цифр или символов в lineEdit_4
    bool isLineEdit4Valid = true;
    QString country = ui->lineEdit_4->text();
    for (QChar c : country) {
        if (c.isDigit()) {
            isLineEdit4Valid = false;
            break;
        }
    }
    if (!isLineEdit4Valid) {
        QMessageBox::warning(this, "Ошибка", "Пожалуйста, введите корректное название страны без цифр.");
        return;
    }

    QString albumName = ui->lineEdit->text();
    QString imagePath;

    if (!imagePathToAdd.isEmpty()) {
        imagePath = "C:/ProjectsQT/MusicManager/Image/" + albumName + ".jpg";

        QFile::rename(imagePathToAdd, imagePath);
    }

    QFile file("C:/ProjectsQT/MusicManager/DataBase/MusicDB.json");
    if (file.open(QIODevice::ReadWrite | QFile::Text))
    {
        QByteArray jsonData = file.readAll();
        QJsonDocument doc = QJsonDocument::fromJson(jsonData);

        QJsonObject jsonObject = doc.object();
        QJsonArray albumsArray = jsonObject["albums"].toArray();

        QVariantMap map;
        map.insert("name", albumName);
        map.insert("artist", ui->lineEdit_2->text());
        map.insert("year", ui->lineEdit_3->text().toInt());
        map.insert("country", ui->lineEdit_4->text());
        map.insert("songs", QJsonArray::fromStringList(QStringList(ui->lineEdit_5->text().split(","))));
        map.insert("cover_photo", imagePath);

        QJsonObject json = QJsonObject::fromVariantMap(map);

        albumsArray.append(json);
        jsonObject["albums"] = albumsArray;

        doc.setObject(jsonObject);

        file.resize(0); // Clear the file
        file.write(doc.toJson());
        file.close();
        close();
    }
    else {
        QMessageBox::warning(this, "Ошибка", "Не удалось открыть файл для записи.");
    }
}

void AddWindow::on_CancelButton_clicked()
{
    close();
}

void AddWindow::on_File_clicked()
{
    QString imagePath = QFileDialog::getOpenFileName(this, "Select Image", "", "Image Files (*.png *.jpg *.jpeg)");

    if (!imagePath.isEmpty()) {
        imagePathToAdd = imagePath;
        QPixmap pixmap(imagePath);
        ui->label_6->setPixmap(pixmap.scaled(ui->label_6->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
}
