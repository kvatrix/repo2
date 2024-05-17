#include "myqtclass.h"
#include <QLabel>
#include <QTime>
#include <QApplication>
#include <QSplashScreen>



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Создаем и показываем окно загрузки
       QLabel *loadingLabel = new QLabel("Loading...");
       loadingLabel->show();


       QTime dieTime = QTime::currentTime().addSecs(2);
       while (QTime::currentTime() < dieTime)
           QCoreApplication::processEvents(QEventLoop::AllEvents, 100);

       loadingLabel->close();



    MyQtClass w;
    w.show();
    return a.exec();
}
