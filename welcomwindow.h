#ifndef WELCOMWINDOW_H
#define WELCOMWINDOW_H


#include <QMainWindow>
#include <QTimer>

class WelcomeWindow : public QMainWindow
{
    Q_OBJECT

public:
    WelcomeWindow(QWidget *parent = nullptr);
    ~WelcomeWindow();

private:
    QTimer *timer; // Таймер для автоматического закрытия окна
};

#endif // WELCOMWINDOW_H
