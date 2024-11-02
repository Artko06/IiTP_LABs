#include "statswindow.h"
#include "ui_statswindow.h"

#include "ConstField.h"
#include "qmessagequestion.h"

const short MAX_PROCENT = 100;

StatsWindow::StatsWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::StatsWindow)
    , updateTimer(new QTimer)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    connect(updateTimer, &QTimer::timeout, this, &StatsWindow::updateStatsOnWindow);
    updateTimer->start(SPEED_TIMER);
}

StatsWindow::~StatsWindow()
{
    delete ui;
}

void StatsWindow::AddStats(bool youWin, bool playWithBot)
{
    QFile file;
    (playWithBot) ? file.setFileName("/home/artem_kokhan/QT/BattleShip/statsWithBot.txt")
                  : file.setFileName("/home/artem_kokhan/QT/BattleShip/statsWithPerson.txt");

    if (!file.open(QIODevice::ReadWrite | QIODevice::Text)) {
        return;
    }

    QTextStream in(&file);
    QStringList lines;
    while (!in.atEnd()) {
        lines.append(in.readLine());
    }

    if (youWin) {
        int num = lines[0].toInt();
        lines[0] = QString::number(num + 1);
    } else {
        int num = lines[1].toInt();
        lines[1] = QString::number(num + 1);
    }

    file.resize(0);
    QTextStream out(&file);
    foreach (QString line, lines) {
        out << line << "\n";
    }
}

void StatsWindow::updateStatsOnWindow()
{
    QFile fileBot, filePerson;
    fileBot.setFileName("/home/artem_kokhan/QT/BattleShip/statsWithBot.txt");
    filePerson.setFileName("/home/artem_kokhan/QT/BattleShip/statsWithPerson.txt");

    if (!fileBot.open(QIODevice::ReadWrite | QIODevice::Text)
        || !filePerson.open(QIODevice::ReadWrite | QIODevice::Text)) {
        return;
    }

    QTextStream inBot(&fileBot), inPerson(&filePerson);
    QStringList linesBot, linesPerson;
    while (!inBot.atEnd()) {
        linesBot.append(inBot.readLine());
        linesPerson.append(inPerson.readLine());
    }

    winWithBot = linesBot.at(0).toInt();
    defeatWithBot = linesBot.at(1).toInt();
    winWithPerson = linesPerson.at(0).toInt();
    defeatWithPerson = linesPerson.at(1).toInt();

    ui->lcdWinWithBot->display(winWithBot);
    ui->lcdDefeatWithBot->display(defeatWithBot);
    ui->lcdWinWithPerson->display(winWithPerson);
    ui->lcdDefeatWithPerson->display(defeatWithPerson);
    ui->lcdAllWithBot->display(winWithBot + defeatWithBot);
    ui->lcdAllWithPerson->display(winWithPerson + defeatWithPerson);
    if (winWithBot) {
        ui->progressBarWithBot->setValue(winWithBot * MAX_PROCENT / (winWithBot + defeatWithBot));
    } else {
        ui->progressBarWithBot->setValue(0);
    }
    if (winWithPerson) {
        ui->progressBarWithPerson->setValue(winWithPerson * MAX_PROCENT
                                            / (winWithPerson + defeatWithPerson));
    } else {
        ui->progressBarWithPerson->setValue(0);
    }
}

void StatsWindow::on_resetStatsBotButton_clicked()
{
    if (QMessageQuestion::setQuestion(
            tr("ВНИМАНИЕ!!!"),
            tr("Вы уверены, что хотите сбросить данные о статистике между Игроком 1 и ботом?"))
            ->exec()
        == QMessageBox::Yes) {
        QFile file("/home/artem_kokhan/QT/BattleShip/statsWithBot.txt");

        if (!file.open(QIODevice::ReadWrite | QIODevice::Text)) {
            return;
        }

        QTextStream in(&file);
        QStringList lines;
        while (!in.atEnd()) {
            lines.append(in.readLine());
        }

        // Устанавливаем значения 0 на первой и второй строках
        if (lines.size() > 0) {
            lines[0] = QString::number(0);
        }
        if (lines.size() > 1) {
            lines[1] = QString::number(0);
        }

        file.resize(0);
        QTextStream out(&file);
        foreach (QString line, lines) {
            out << line << "\n";
        }
    }
}

void StatsWindow::on_resetStatsPersonButton_clicked()
{
    if (QMessageQuestion::setQuestion(
            tr("ВНИМАНИЕ!!!"),
            tr("Вы уверены, что хотите сбросить данные о статистике между Игроком 1 и Игроком 2?"))
            ->exec()
        == QMessageBox::Yes) {
        QFile file("/home/artem_kokhan/QT/BattleShip/statsWithPerson.txt");

        if (!file.open(QIODevice::ReadWrite | QIODevice::Text)) {
            return;
        }

        QTextStream in(&file);
        QStringList lines;
        while (!in.atEnd()) {
            lines.append(in.readLine());
        }

        // Устанавливаем значения 0 на первой и второй строках
        if (lines.size() > 0) {
            lines[0] = QString::number(0);
        }
        if (lines.size() > 1) {
            lines[1] = QString::number(0);
        }

        file.resize(0);
        QTextStream out(&file);
        foreach (QString line, lines) {
            out << line << "\n";
        }
    }
}
