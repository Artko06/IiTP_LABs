#include "fieldsecondplayer.h"
#include "ui_fieldsecondplayer.h"

#include <QProcess>
#include "ConstField.h"
#include "addships.h"
#include "correctposship.h"
#include "lettersonwindow.h"
#include "qmessagequestion.h"

FieldSecondPlayer::FieldSecondPlayer(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::FieldSecondPlayer)
    , battlefieldPerson(new Battlefield)
    , sceneSecondPlayer(new BattleScene)
    , buildBattleFieldPlayer2(new BuildBattleField(sceneSecondPlayer))
    , errorFieldTimer(new QTimer)
{
    ui->setupUi(this);

    ui->graphicsView->setScene(sceneSecondPlayer);
    sceneSecondPlayer->setSceneRect(0, 0, sizeBattleField, sizeBattleField);
    LettersOnWindow::setLetters(this, ui->graphicsView->pos());

    errorFieldTimer->start(SPEED_TIMER);
    connect(errorFieldTimer, &QTimer::timeout, this, &FieldSecondPlayer::searchErrorField);
}

FieldSecondPlayer::~FieldSecondPlayer()
{
    delete ui;
}

void FieldSecondPlayer::setNicknamePerson1(const QString nicknamePerson1)
{
    this->nicknamePerson1 = nicknamePerson1;
}

void FieldSecondPlayer::setSceneFirstPlayerOnSecondWindow(BattleScene *scene)
{
    this->sceneFirstPlayerOnSecondWindow = scene;
}

void FieldSecondPlayer::on_startButton_clicked()
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
        if (QMessageQuestion::setQuestion(tr("Начало игры"),
                                          tr("Вы действительно хотите начать игру? Расположение "
                                             "кораблей в дальнейшем будет невозможно изменить"))
                ->exec()
            == QMessageBox::Yes) {
            this->close();

            if (!this->nicknamePerson1.isEmpty()) {
                battlefieldPerson->getFirstLabel()->setText(this->nicknamePerson1);
            }

            if (!ui->nicknameEdit->text().isEmpty()) {
                battlefieldPerson->getSecondLabel()->setText(ui->nicknameEdit->text());
            }

            Ship::disMove();
            battlefieldPerson->setBattleFirstScene(sceneFirstPlayerOnSecondWindow);
            if (sceneFirstPlayerOnSecondWindow != nullptr) {
                battlefieldPerson->setBattleSecondScene(sceneSecondPlayer);
            }
            sceneSecondPlayer->listShips = AddShips::getListShips();
            sceneSecondPlayer->convertToPos();
            hideShipsBeforeTheFight(sceneSecondPlayer);
            battlefieldPerson->showFullScreen();
            errorFieldTimer->stop();
        }
    }
}

void FieldSecondPlayer::on_menuButton_clicked()
{
    if (QMessageQuestion::setQuestion(
            tr("Выxoд в главное меню"),
            tr("Вы действительно хотите выйти в главное меню? Ваш прогресс будет ПОТЕРЯН!!!"))
            ->exec()
        == QMessageBox::Yes) {
        qApp->quit();
        QProcess::startDetached(qApp->arguments()[0], qApp->arguments().mid(1));
    }
}

void FieldSecondPlayer::searchErrorField()
{
    CorrectPosShip::correctPosShip(buildBattleFieldPlayer2);
}

void FieldSecondPlayer::on_autoButton_clicked()
{
    AddShips::autoPlacing(sceneSecondPlayer);
    ui->firstShipButton->setText(" - " + QString::number(0));
    ui->secondShipButton->setText(" - " + QString::number(0));
    ui->thirdShipButton->setText(" - " + QString::number(0));
    ui->forthShipButton->setText(" - " + QString::number(0));
}

void FieldSecondPlayer::on_firstShipButton_clicked()
{
    QChar amountShips = ui->firstShipButton->text().back();

    if (amountShips.digitValue() != 0) {
        AddShips::addShipsToScene(0, 0, TypeShip::FirstShip, sceneSecondPlayer);
        ui->firstShipButton->setText(" - " + QString::number(amountShips.digitValue() - 1));
    }
}

void FieldSecondPlayer::on_secondShipButton_clicked()
{
    QChar amountShips = ui->secondShipButton->text().back();

    if (amountShips.digitValue() != 0) {
        AddShips::addShipsToScene(0, 0, TypeShip::SecondShip, sceneSecondPlayer);
        ui->secondShipButton->setText(" - " + QString::number(amountShips.digitValue() - 1));
    }
}

void FieldSecondPlayer::on_thirdShipButton_clicked()
{
    QChar amountShips = ui->thirdShipButton->text().back();

    if (amountShips.digitValue() != 0) {
        AddShips::addShipsToScene(0, 0, TypeShip::ThirdShip, sceneSecondPlayer);
        ui->thirdShipButton->setText(" - " + QString::number(amountShips.digitValue() - 1));
    }
}

void FieldSecondPlayer::on_forthShipButton_clicked()
{
    QChar amountShips = ui->forthShipButton->text().back();

    if (amountShips.digitValue() != 0) {
        AddShips::addShipsToScene(0, 0, TypeShip::ForthShip, sceneSecondPlayer);
        ui->forthShipButton->setText(" - " + QString::number(amountShips.digitValue() - 1));
    }
}

void FieldSecondPlayer::hideShipsBeforeTheFight(BattleScene *scene)
{
    QVector<QGraphicsItem *> foundItems = scene->items();
    for (QGraphicsItem *item : foundItems) {
        if (Ship *ship = dynamic_cast<Ship *>(item)) {
            ship->setVisible(false);
        }
    }
}
