#include "fieldfirstplayer.h"
#include "ui_fieldfirstplayer.h"

#include <QLabel>
#include "ConstField.h"
#include "addships.h"
#include "correctposship.h"
#include "lettersonwindow.h"
#include "qmessagequestion.h"

FieldFirstPlayer::FieldFirstPlayer(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::FieldFirstPlayer)
    , fieldSecondPlayer(new FieldSecondPlayer)
    , battlefieldBot(new Battlefield)
    , sceneFirstPlayer(new BattleScene)
    , sceneBot(new BattleScene)
    , buildBattleFieldPlayer1(new BuildBattleField(sceneFirstPlayer))
    , buildBattleFieldBot(new BuildBattleField(sceneBot))
    , errorFieldTimer(new QTimer)
{
    ui->setupUi(this);

    ui->graphicsView->setScene(sceneFirstPlayer);
    sceneFirstPlayer->setSceneRect(0, 0, sizeBattleField, sizeBattleField);
    LettersOnWindow::setLetters(this, ui->graphicsView->pos());
    errorFieldTimer->start(SPEED_TIMER);
    connect(errorFieldTimer, &QTimer::timeout, this, &FieldFirstPlayer::searchErrorField);
}

FieldFirstPlayer::~FieldFirstPlayer()
{
    delete ui;
}

void FieldFirstPlayer::personButtonChoise()
{
    ui->startButton->setText("NEXT");
}

void FieldFirstPlayer::botButtonChoise()
{
    ui->startButton->setText("START");
}

void FieldFirstPlayer::on_startButton_clicked()
{
    if (CorrectPosShip::getErrorOneOfShips()) {
        QMessageBox::critical(this,
                              tr("ОШИБКА"),
                              tr("Ошибка расстановки кораблей! Корабли стоят слишком близко"));
    }

    else if (ui->firstShipButton->text() != " - 0" || ui->secondShipButton->text() != " - 0"
             || ui->thirdShipButton->text() != " - 0" || ui->forthShipButton->text() != " - 0"
             || !CorrectPosShip::allShipsOnField()) {
        QMessageBox::critical(this,
                              tr("ОШИБКА"),
                              tr("Ошибка расстановки кораблей! Не все корабли были расставлены"));
    }

    else {
        if (ui->startButton->text() == "NEXT") {
            if (QMessageQuestion::setQuestion(
                    tr("Передача хода"),
                    tr("Вы действительно хотите передать ход для расставления "
                       "короблей второму игроку? Расположение кораблей в "
                       "дальнейшем будет невозможно изменить"))
                    ->exec()
                == QMessageBox::Yes) {
                this->close();
                fieldSecondPlayer->setNicknamePerson1(ui->nicknameEdit->text());
                fieldSecondPlayer->setSceneFirstPlayerOnSecondWindow(sceneFirstPlayer);
                sceneFirstPlayer->listShips = AddShips::getListShips();
                sceneFirstPlayer->convertToPos();
                fieldSecondPlayer->showFullScreen();
                hideShipsBeforeTheFight(sceneFirstPlayer);
                AddShips::deleteListOfShips();
                errorFieldTimer->stop();
            }
        }

        else {
            if (QMessageQuestion::setQuestion(
                    tr("Начало игры"),
                    tr("Вы действительно хотите начать игру? Расположение "
                       "кораблей в дальнейшем будет невозможно изменить"))
                    ->exec()
                == QMessageBox::Yes) {
                this->close();

                if (!ui->nicknameEdit->text().isEmpty()) {
                    battlefieldBot->getFirstLabel()->setText(ui->nicknameEdit->text());
                }

                battlefieldBot->getSecondLabel()->setText("Bot");
                battlefieldBot->setBattleFirstScene(sceneFirstPlayer);
                battlefieldBot->setBattleSecondScene(sceneBot);
                Ship::disMove();
                sceneFirstPlayer->listShips = AddShips::getListShips();
                sceneFirstPlayer->convertToPos();
                AddShips::autoPlacing(sceneBot);
                sceneBot->listShips = AddShips::getListShips();
                sceneBot->convertToPos();
                battlefieldBot->showFullScreen();
                battlefieldBot->goPlayWithBot();
                hideShipsBeforeTheFight(sceneBot);
                errorFieldTimer->stop();
            }
        }
    }
}

void FieldFirstPlayer::on_firstShipButton_clicked()
{
    QChar amountShips = ui->firstShipButton->text().back();

    if (amountShips.digitValue() != 0) {
        AddShips::addShipsToScene(0, 0, TypeShip::FirstShip, sceneFirstPlayer);
        ui->firstShipButton->setText(" - " + QString::number(amountShips.digitValue() - 1));
    }
}

void FieldFirstPlayer::on_secondShipButton_clicked()
{
    QChar amountShips = ui->secondShipButton->text().back();

    if (amountShips.digitValue() != 0) {
        AddShips::addShipsToScene(0, 0, TypeShip::SecondShip, sceneFirstPlayer);
        ui->secondShipButton->setText(" - " + QString::number(amountShips.digitValue() - 1));
    }
}

void FieldFirstPlayer::on_thirdShipButton_clicked()
{
    QChar amountShips = ui->thirdShipButton->text().back();

    if (amountShips.digitValue() != 0) {
        AddShips::addShipsToScene(0, 0, TypeShip::ThirdShip, sceneFirstPlayer);
        ui->thirdShipButton->setText(" - " + QString::number(amountShips.digitValue() - 1));
    }
}

void FieldFirstPlayer::on_forthShipButton_clicked()
{
    QChar amountShips = ui->forthShipButton->text().back();

    if (amountShips.digitValue() != 0) {
        AddShips::addShipsToScene(0, 0, TypeShip::ForthShip, sceneFirstPlayer);
        ui->forthShipButton->setText(" - " + QString::number(amountShips.digitValue() - 1));
    }
}

void FieldFirstPlayer::searchErrorField()
{
    CorrectPosShip::correctPosShip(buildBattleFieldPlayer1);
}

void FieldFirstPlayer::on_autoButton_clicked()
{
    AddShips::autoPlacing(sceneFirstPlayer);
    ui->firstShipButton->setText(" - " + QString::number(0));
    ui->secondShipButton->setText(" - " + QString::number(0));
    ui->thirdShipButton->setText(" - " + QString::number(0));
    ui->forthShipButton->setText(" - " + QString::number(0));
}

void FieldFirstPlayer::hideShipsBeforeTheFight(BattleScene *scene)
{
    QVector<QGraphicsItem *> foundItems = scene->items();
    for (QGraphicsItem *item : foundItems) {
        if (Ship *ship = dynamic_cast<Ship *>(item)) {
            ship->setVisible(false);
        }
    }
}
