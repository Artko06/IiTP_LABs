#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "settingwindow.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void menuButton_from_setting();

    void on_quitButton_clicked();

    void on_startButton_clicked();

    void on_infoButton_clicked();

private:
    Ui::MainWindow *ui;
    SettingWindow *settingUi;
};
#endif // MAINWINDOW_H
