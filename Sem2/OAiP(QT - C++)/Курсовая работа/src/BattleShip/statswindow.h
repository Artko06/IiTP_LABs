#ifndef STATSWINDOW_H
#define STATSWINDOW_H

#include <QFile>
#include <QMainWindow>
#include <QTextStream>
#include <QTimer>

namespace Ui {
class StatsWindow;
}

class StatsWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit StatsWindow(QWidget *parent = nullptr);
    ~StatsWindow();
    static void AddStats(bool youWin, bool playWithBot);
    void updateStatsOnWindow();

private slots:
    void on_backButton_clicked() { emit backSignalStatsWindow(); };

    void on_resetStatsBotButton_clicked();

    void on_resetStatsPersonButton_clicked();

signals:
    void backSignalStatsWindow();

private:
    Ui::StatsWindow *ui;
    QTimer *updateTimer;
    int winWithBot, winWithPerson;
    int defeatWithBot, defeatWithPerson;
};

#endif // STATSWINDOW_H
