#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QKeyEvent>
#include <QMainWindow>
#include "Unordered_Map.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    void extracted();
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_clearButton_clicked();

private:
    void keyPressEvent(QKeyEvent *event) override;

private:
    Ui::MainWindow *ui;
    Unordered_Map<int, QString> unordered_map;
    std::pair<int, QString> temp;
};
#endif // MAINWINDOW_H
