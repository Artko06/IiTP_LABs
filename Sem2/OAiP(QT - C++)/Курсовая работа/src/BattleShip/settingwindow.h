#ifndef SETTINGWINDOW_H
#define SETTINGWINDOW_H

#include "fieldfirstplayer.h"
#include "statswindow.h"

namespace Ui {
class SettingWindow;
}

class SettingWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit SettingWindow(QWidget *parent = nullptr);
    ~SettingWindow();

signals:
    void menuSignal();
    void personButtonClick();
    void botButtonClick();

private slots:
    void backButton_from_firstField();

    void backButton_from_statsWindow();

    void on_menuButton_clicked(){emit menuSignal();};

    void on_botButton_clicked();

    void on_personButton_clicked();

    void on_statsButton_clicked();

private:
    Ui::SettingWindow *ui;
    FieldFirstPlayer* fieldFirstPlayer;
    StatsWindow *statsWindow;
};

#endif // SETTINGWINDOW_H
