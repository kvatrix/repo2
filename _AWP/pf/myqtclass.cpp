#include <QRandomGenerator>
#include <QTimer>
#include "myqtclass.h"
#include "ui_myqtclass.h"

#include <QImage>
#include <QPixmap>
#include <QPainter>
#include <QFileDialog>
#include <QDir>
#include <QDebug>



MyQtClass::MyQtClass(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MyQtClass)
{
    ui->setupUi(this);

    connect(ui->collagebtn_ver,    &QPushButton::clicked,  this,   &MyQtClass::collage_ver);  //кнопка коллажа по вертикали
    connect(ui->collagebtn_hor,    &QPushButton::clicked,  this,   &MyQtClass::collage_hor);  //кнопка коллажа по горизонтали

    connect(ui->collagebtn_ver_2,    &QPushButton::clicked,  this,   &MyQtClass::collage_ver_2);  //кнопка коллажа по вертикали 2
    connect(ui->collagebtn_hor_2,    &QPushButton::clicked,  this,   &MyQtClass::collage_hor_2);  //кнопка коллажа по горизонтали 2

/*  connect(ui->userinput,  &QLineEdit::textChanged,    this,   &MyQtClass::on_userInput_textChanged);
  connect(ui->usertextbtr,    &QPushButton::clicked,  this,   &MyQtClass::changeText);
   QTimer *timer = new QTimer(this);
   connect(timer,  &QTimer::timeout,    this,   &MyQtClass:: updatePressure);
    timer->start(44);
  connect(this,   &MyQtClass::pressureChanged,  this,   &MyQtClass::on_PressureChanged);
*/

}


void MyQtClass::collage_ver(){

    // Выбираем изображения для создания коллажа
    QStringList imagePaths = QFileDialog::getOpenFileNames(nullptr, "Выберите изображения", QDir::homePath(), "Images (*.png *.jpg)");

    if (imagePaths.size() == 0) {
        qDebug() << "Нет выбранных изображений";
        return;
    }


    // Вычисляем общую высоту и ширину для коллажа
    int collageWidth = 1080; // Минимальная ширина
    int collageHeight = 0;

    foreach (QString imagePath, imagePaths) {
        QImage image(imagePath);
        collageHeight += image.height();
        if (image.width() > collageWidth) {
            collageWidth = image.width(); // Обновляем ширину, если обнаружен более широкий образ
        }
    }

    // Создаем QImage для коллажа
    QImage collage(collageWidth, collageHeight, QImage::Format_ARGB32);
    collage.fill(Qt::white);

    // Создаем QPainter для рисования на коллаже
    QPainter painter(&collage);

    int y = 0;

    // Рисуем каждое изображение на коллаже
    foreach (QString imagePath, imagePaths) {
        QImage image(imagePath);
        int imageWidth = image.width();
        int imageHeight = image.height();

        painter.drawImage(QPoint((collageWidth - imageWidth) / 2, y), image);
        y += imageHeight;
    }

    // Сохраняем коллаж в файл
    QString collagePath = QFileDialog::getSaveFileName(nullptr, "Сохранить коллаж", QDir::homePath(), "Images (*.png)");
    collage.save(collagePath);
}

void MyQtClass::collage_hor(){


    // Выбираем изображения для создания коллажа
    QStringList imagePaths = QFileDialog::getOpenFileNames(nullptr, "Выберите изображения", QDir::homePath(), "Images (*.png *.jpg)");

    if (imagePaths.size() == 0) {
        qDebug() << "Нет выбранных изображений";
        return;
    }

    // Вычисляем общую высоту и ширину для коллажа
    int collageHeight = 0; // Минимальная высота
    int collageWidth = 0;

    foreach (QString imagePath, imagePaths) {
        QImage image(imagePath);
        collageWidth += image.width();
        if (image.height() > collageHeight) {
            collageHeight = image.height(); // Обновляем высоту, если обнаружена более высокая картинка
        }
    }


    // Создаем QImage для коллажа
    QImage collage(collageWidth, collageHeight, QImage::Format_ARGB32);
    collage.fill(Qt::white);

    // Создаем QPainter для рисования на коллаже
    QPainter painter(&collage);

    int x = 0;

    // Рисуем каждое изображение на коллаже
    foreach (QString imagePath, imagePaths) {
        QImage image(imagePath);
        int imageWidth = image.width();
        int imageHeight = image.height();

        painter.drawImage(QPoint(x, (collageHeight - imageHeight) / 2), image);
        x += imageWidth;
    }

    // Сохраняем коллаж в файл
    QString collagePath = QFileDialog::getSaveFileName(nullptr, "Сохранить коллаж", QDir::homePath(), "Images (*.png)");
    collage.save(collagePath);

}

void MyQtClass::collage_ver_2(){

    // Выбираем изображения для создания коллажа
       QStringList imagePaths = QFileDialog::getOpenFileNames(nullptr, "Выберите изображения", QDir::homePath(), "Images (*.png *.jpg)");

       if (imagePaths.size() == 0) {
           qDebug() << "Нет выбранных изображений";

       }

       // Проверяем, что общее количество изображений делится на 2
       if (imagePaths.size() % 2 != 0) {
           qDebug() << "Количество изображений должно быть кратно двум для создания двух колонн";

       }

       // Вычисляем количество изображений в каждой колонне
       int imagesPerColumn = imagePaths.size() / 2;

       // Вычисляем ширину коллажа по максимальной ширине изображений в каждой колонне
       int maxColumnWidth = 0;
       for (int i = 0; i < imagesPerColumn; ++i) {
           QImage image(imagePaths.at(i));
           if (image.width() > maxColumnWidth) {
               maxColumnWidth = image.width();
           }
       }
       int collageWidth = maxColumnWidth * 2;

       // Вычисляем высоту коллажа
       int collageHeight = 0;
       for (int i = 0; i < imagePaths.size()/2; ++i) {
           QImage image(imagePaths.at(i));
           collageHeight += image.height();
       }
   //    collageHeight = collageHeight/2;

       // Создаем QImage для коллажа
       QImage collage(collageWidth, collageHeight, QImage::Format_ARGB32);
       collage.fill(Qt::white);

       // Создаем QPainter для рисования на коллаже
       QPainter painter(&collage);

       int x = 0;
       int y = 0;

       // Рисуем изображения в двух колоннах
       for (int col = 0; col < 2; ++col) {
           for (int j = 0; j < imagesPerColumn; ++j) {
               QImage image(imagePaths.at(col * imagesPerColumn + j));
               painter.drawImage(x, y, image);
               y += image.height();
           }
           x += maxColumnWidth;
           y = 0; // Сброс координаты y для новой колонны
       }

       // Сохраняем коллаж в файл
       QString collagePath = QFileDialog::getSaveFileName(nullptr, "Сохранить коллаж", QDir::homePath(), "Images (*.png)");
       collage.save(collagePath);

}

void MyQtClass::collage_hor_2(){

    // Выбор изображений для коллажа
       QStringList imagePaths = QFileDialog::getOpenFileNames(nullptr, "Выберите изображения", QDir::homePath(), "Images (*.png *.jpg)");

       if (imagePaths.size() == 0) {
           qDebug() << "Нет выбранных изображений";

       }

       // Убеждаемся, что количество изображений делится на 2
       if (imagePaths.size() % 2 != 0) {
           qDebug() << "Количество изображений должно быть кратно двум для создания двух строк";

       }

       int imagesPerRow = imagePaths.size() / 2; // количество изображений в каждой строке

       // Вычисляем максимальную ширину и высоту изображений
       int maxImageWidth = 0, maxImageHeight = 0;
       for (int i = 0; i < imagesPerRow; ++i) {
           QImage image1(imagePaths.at(i));
           QImage image2(imagePaths.at(i + imagesPerRow));

           maxImageWidth = qMax(maxImageWidth, qMax(image1.width(), image2.width()));
           maxImageHeight = qMax(maxImageHeight, qMax(image1.height(), image2.height()));
       }

       int collageWidth = maxImageWidth * imagesPerRow;
       int collageHeight = 2 * maxImageHeight;

       // Создаем QImage для коллажа
       QImage collage(collageWidth, collageHeight, QImage::Format_ARGB32);
       collage.fill(Qt::white);

       // Создаем QPainter для рисования на коллаже
       QPainter painter(&collage);

       int x = 0, y = 0;

       // Рисуем изображения в двух строках
       for (int i = 0; i < imagesPerRow; ++i) {
           QImage image1(imagePaths.at(i));
           QImage image2(imagePaths.at(i + imagesPerRow));

           painter.drawImage(x, y, image1);
           painter.drawImage(x + maxImageWidth, y, image2);

           y += maxImageHeight;
       }

       // Сохраняем коллаж в файл
       QString collagePath = QFileDialog::getSaveFileName(nullptr, "Сохранить коллаж", QDir::homePath(), "Images (*.png)");
       collage.save(collagePath);

}

MyQtClass::~MyQtClass()
{
    delete ui;
}


/*
void MyQtClass::changeText(){
    ui->usertext->setText("Привет!");
    ui->usertext->setGeometry(0, 0, 1200, 150);
    const QString s = ui->userinput->text();
}

void MyQtClass::on_userInput_textChanged(const QString &arg1){

    ui->usertext->setText(arg1);
}

void MyQtClass::updatePressure(){
    float newRandomPressure =
            QRandomGenerator::global()->bounded(50);
    emit pressureChanged(newRandomPressure);
}

void MyQtClass::on_PressureChanged(const float newValue){
    ui->pressureDisplay->display(newValue);
}

*/

