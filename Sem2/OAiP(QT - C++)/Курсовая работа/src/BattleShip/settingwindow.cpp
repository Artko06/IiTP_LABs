#include "settingwindow.h"
#include "ui_settingwindow.h"

SettingWindow::SettingWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::SettingWindow)
    , fieldFirstPlayer(new FieldFirstPlayer)
    , statsWindow(new StatsWindow)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    connect(fieldFirstPlayer, &FieldFirstPlayer::backSignalFirstWindow, this, &SettingWindow::backButton_from_firstField);
    connect(statsWindow,
            &StatsWindow::backSignalStatsWindow,
            this,
            &SettingWindow::backButton_from_statsWindow);
    connect(this, &SettingWindow::personButtonClick, fieldFirstPlayer, &FieldFirstPlayer::personButtonChoise);
    connect(this, &SettingWindow::botButtonClick, fieldFirstPlayer, &FieldFirstPlayer::botButtonChoise);
}

SettingWindow::~SettingWindow()
{
    delete ui;
}

void SettingWindow::backButton_from_firstField()
{
    fieldFirstPlayer->close();
    this->showFullScreen();
}

void SettingWindow::backButton_from_statsWindow()
{
    statsWindow->close();
    this->showFullScreen();
}

void SettingWindow::on_botButton_clicked()
{
    this->close();
    fieldFirstPlayer->showFullScreen();
    emit botButtonClick();
}


void SettingWindow::on_personButton_clicked()
{
    this->close();
    fieldFirstPlayer->showFullScreen();
    emit personButtonClick();
}

void SettingWindow::on_statsButton_clicked()
{
    this->close();
    statsWindow->showFullScreen();
}
