#include "welcomwindow.h"
#include <QProgressBar>
#include <QLabel>
#include <QVBoxLayout>
#include <QTimer>
#include <QPixmap>
#include <QPalette>
#include <QFont>
#include <QFontDatabase>

WelcomeWindow::WelcomeWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->setFixedSize(700, 400);
    this->setWindowFlags(Qt::FramelessWindowHint);

    QPixmap background(":/AppImage/bg.png");
    background = background.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Background, background);
    this->setPalette(palette);

    QTimer *timer = new QTimer(this);
    timer->setInterval(20);

    QProgressBar *progressBar = new QProgressBar(this);
    progressBar->setRange(0, 100);
    progressBar->setTextVisible(false);
    progressBar->setStyleSheet("QProgressBar{height:24px; text-align:center; font-size:16px; color:white; border-radius:11px; background:#1D5573;}"
                                       "QProgressBar::chunk{border-radius:11px;background:qlineargradient(spread:pad,x1:0,y1:0,x2:1,y2:0,stop:0 #9efff9,stop:1 #73c98e);}");

    QLabel *welcomeLabel = new QLabel("MusicManager", this);
    welcomeLabel->setAlignment(Qt::AlignCenter);
    QFont font("Calibri", 42, QFont::Bold);
    welcomeLabel->setFont(font);

    QPalette textColor;
    textColor.setColor(QPalette::WindowText, QColor("#73c98e"));
    welcomeLabel->setPalette(textColor);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(welcomeLabel);
    layout->addWidget(progressBar);

    QWidget *centralWidget = new QWidget(this);
    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);

    connect(timer, &QTimer::timeout, [=]() {
        static int progress = 0;
        progress += 1;
        progressBar->setValue(progress);

        if(progress >= 100) {
            this->close();
        }
    });
    timer->start();
}

WelcomeWindow::~WelcomeWindow()
{
    timer->stop();
    delete timer;
}
