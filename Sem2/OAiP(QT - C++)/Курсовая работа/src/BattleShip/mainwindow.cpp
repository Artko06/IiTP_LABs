#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "qmessagequestion.h"

constexpr short FONT_FOR_THE_INFO_GAME = 18;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , settingUi(new SettingWindow)
{
    ui->setupUi(this);
    connect(settingUi, &SettingWindow::menuSignal, this, &MainWindow::menuButton_from_setting);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::menuButton_from_setting()
{
    settingUi->close();
    this->showFullScreen();
}

void MainWindow::on_quitButton_clicked()
{
    if (QMessageQuestion::setQuestion(tr("Выход"), tr("Вы действительно хотите выйти из игры?"))
            ->exec()
        == QMessageBox::Yes) {
        QCoreApplication::quit();
    }
}


void MainWindow::on_startButton_clicked()
{
    this->close();
    settingUi->showFullScreen();
}

void MainWindow::on_infoButton_clicked()
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("Правила игры");
    msgBox.setText(
        "Вначале перед мореходами стоит стратегически важная задача, от решения которой "
        "зависит, возможно, весь исход игры: расположить корабли на поле таким образом, чтобы "
        "противнику как можно сложнее было распознать их местонахождение. Суда могут быть "
        "расставлены в хаотичном порядке (горизонтально, вертикально или даже по краю поля), "
        "главное, чтобы между ними было расстояние не менее одной клетки. Далее игроки по "
        "очереди стреляют,"
        "eсли в тебя попали или ты совершил попадание, то на поле можно уведеть красный крестик "
        "на этом месте. "
        "Если угаданы все клетки одного корабля, то корабль будет считаться униченоженым, это "
        "можно будет увидеть как зачёркнутые клетки корабля. "
        "Если игрок попал в цель, он продолжает стрелять до "
        "первого промаха. Игра считается "
        "оконченной, как только будут повержены все корабли одного флота.");

    QFont font = msgBox.font();
    font.setPointSize(FONT_FOR_THE_INFO_GAME);
    msgBox.setFont(font);
    msgBox.setIcon(QMessageBox::Information);
    msgBox.exec();
}
