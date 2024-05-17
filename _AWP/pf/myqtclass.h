#ifndef MYQTCLASS_H
#define MYQTCLASS_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MyQtClass; }
QT_END_NAMESPACE

class MyQtClass : public QMainWindow
{
    Q_OBJECT

public:
    MyQtClass(QWidget *parent = nullptr);
    ~MyQtClass();

signals:
    void pressureChanged(float currentValue);

public slots:


    void collage_hor();

    void collage_ver();

    void collage_hor_2();

    void collage_ver_2();

/*
    void on_userInput_textChanged(const QString &arg1);
    void changeText();
    void on_PressureChanged(const float newValue);

    void updatePressure();
*/



private:
    Ui::MyQtClass *ui;
};
#endif // MYQTCLASS_H
