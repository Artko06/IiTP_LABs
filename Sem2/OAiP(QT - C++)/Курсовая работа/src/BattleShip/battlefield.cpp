#include "battlefield.h"
#include "ui_battlefield.h"

#include <QApplication>
#include <QProcess>
#include "ConstField.h"
#include "ConstShips.h"
#include "buildbattlefield.h"
#include "lettersonwindow.h"
#include "qmessagequestion.h"
#include "statswindow.h"

Battlefield::Battlefield(QWidget *parent)
    : QMainWindow(parent)
    , missImage(new QPixmap(":/image/image/miss.png"))
    , hitImage(new QPixmap(":/image/image/hit.png"))
    , killImage(new QPixmap(":/image/image/kill.png"))
    , aimImage(new QPixmap(":/image/image/Aim.png"))
    , ui(new Ui::Battlefield)
    , battleFirstScene(new BattleScene)
    , battleSecondScene(new BattleScene)
    , buildBattleFieldFirstScene(new BuildBattleField(battleFirstScene))
    , buildBattleFieldSecondScene(new BuildBattleField(battleSecondScene))
    , bot(new Bot)
    , delayTimerGenerate(new QTimer)
    , delayTimerShot(new QTimer)
    , checkFailClick(new QTimer)
{
    ui->setupUi(this);
    ui->redWinLabel->setVisible(false);
    ui->greenWinLabel->setVisible(false);
    ui->graphicsView_2->setScene(battleSecondScene);
    battleFirstScene->setSceneRect(0, 0, sizeBattleField, sizeBattleField);
    battleSecondScene->setSceneRect(0, 0, sizeBattleField, sizeBattleField);
    LettersOnWindow::setLetters(this, ui->graphicsView->pos());
    LettersOnWindow::setLetters(this, ui->graphicsView_2->pos());

    connect(battleSecondScene, &BattleScene::sceneClicked, this, &Battlefield::onSceneClicked);
    connect(delayTimerGenerate, &QTimer::timeout, this, &Battlefield::generateBotNumber);
    connect(delayTimerShot, &QTimer::timeout, this, &Battlefield::botShoting);
    connect(checkFailClick, &QTimer::timeout, this, &Battlefield::checkFailedClick);
    checkFailClick->start(SPEED_TIMER);
}

Battlefield::~Battlefield()
{
    delete ui;
}

QLabel* Battlefield::getFirstLabel() const
{
    return this->ui->firstPlayerLabel;
}

QLabel *Battlefield::getSecondLabel() const
{
    return this->ui->secondPlayerLabel;
}

void Battlefield::setBattleFirstScene(BattleScene *scene)
{
    battleFirstScene = scene;
    ui->graphicsView->setScene(battleFirstScene);
    connect(battleFirstScene, &BattleScene::sceneClicked, this, &Battlefield::onSceneClicked);
}

void Battlefield::setBattleSecondScene(BattleScene *scene)
{
    battleSecondScene = scene;
    ui->graphicsView_2->setScene(battleSecondScene);
    connect(battleSecondScene, &BattleScene::sceneClicked, this, &Battlefield::onSceneClicked);
}

void Battlefield::goPlayWithBot()
{
    this->playWithBot = true;
}

void Battlefield::firstPlayerMissedShot()
{
    ui->lcdShip->display(ui->lcdShip->value() - 1);
}

void Battlefield::secondPlayerMissedShot()
{
    ui->lcdShip_2->display(ui->lcdShip_2->value() - 1);
}

void Battlefield::firstPlayerDidStep()
{
    ui->lcdStep->display(ui->lcdStep->value() + 1);
}

void Battlefield::secondPlayerDidStep()
{
    ui->lcdStep_2->display(ui->lcdStep_2->value() + 1);
}

void Battlefield::on_menuButton_clicked()
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

void Battlefield::onSceneClicked(BattleScene *scene, QPointF clickedPos)
{
    QGraphicsPixmapItem *image = new QGraphicsPixmapItem();
    image->setZValue(ZValueForClickOnScene);

    if (scene == battleFirstScene && !firstPlayerIsShoting && !playWithBot) {
        handleFirstSceneClick(scene, clickedPos, image);
    } else if (scene == battleSecondScene && firstPlayerIsShoting) {
        handleSecondSceneClick(scene, clickedPos, image);
    }

    if (playWithBot && !firstPlayerIsShoting) {
        this->setEnabled(false);
        delayTimerGenerate->start(SPEED_AIM_TIMER);
    }
}

void Battlefield::handleFirstSceneClick(BattleScene *scene,
                                        QPointF clickedPos,
                                        QGraphicsPixmapItem *image)
{
    if (isShotValid(scene, clickedPos)) {
        if (isHit(scene, clickedPos)) {
            handleHit(scene, clickedPos, image);
        } else {
            handleMiss(scene, clickedPos, image);
        }
        secondPlayerDidStep();
    }
}

void Battlefield::handleSecondSceneClick(BattleScene *scene,
                                         QPointF clickedPos,
                                         QGraphicsPixmapItem *image)
{
    if (isShotValid(scene, clickedPos)) {
        if (isHit(scene, clickedPos)) {
            handleHit(scene, clickedPos, image);
        } else {
            handleMiss(scene, clickedPos, image);
        }
        firstPlayerDidStep();
    }
}

bool Battlefield::isShotValid(BattleScene *scene, QPointF clickedPos)
{
    return scene->finalPosShips[clickedPos.y() / SIZE_ONE_SELL + 1]
                               [clickedPos.x() / SIZE_ONE_SELL + 1]
           < deltaShot;
}

bool Battlefield::isHit(BattleScene *scene, QPointF clickedPos)
{
    return scene->finalPosShips[clickedPos.y() / SIZE_ONE_SELL + 1]
                               [clickedPos.x() / SIZE_ONE_SELL + 1]
           != 0;
}

void Battlefield::handleHit(BattleScene *scene, QPointF clickedPos, QGraphicsPixmapItem *image)
{
    (scene == battleFirstScene) ? firstPlayerMissedShot() : secondPlayerMissedShot();
    setImageProperties(image, hitImage, clickedPos);
    scene->addItem(image);
    incrementShotValue(scene, clickedPos);
    checkToKill(scene, getShotValue(scene, clickedPos) - deltaShot);
}

void Battlefield::handleMiss(BattleScene *scene, QPointF clickedPos, QGraphicsPixmapItem *image)
{
    setImageProperties(image, missImage, clickedPos);
    scene->addItem(image);
    firstPlayerIsShoting = !firstPlayerIsShoting;
    ui->imageHand->setStyleSheet(firstPlayerIsShoting ? "image: url(:/image/image/rightHand.png)"
                                                      : "image: url(:/image/image/leftHand.png)");
    incrementShotValue(scene, clickedPos);
}

void Battlefield::setImageProperties(QGraphicsPixmapItem *image, QPixmap *pixmap, QPointF clickedPos)
{
    image->setPixmap(pixmap->scaled(SIZE_ONE_SELL, SIZE_ONE_SELL, Qt::KeepAspectRatio));
    image->moveBy(clickedPos.x(), clickedPos.y());
}

void Battlefield::incrementShotValue(BattleScene *scene, QPointF clickedPos)
{
    scene->finalPosShips[clickedPos.y() / SIZE_ONE_SELL + 1][clickedPos.x() / SIZE_ONE_SELL + 1]
        += deltaShot;
}

int Battlefield::getShotValue(BattleScene *scene, QPointF clickedPos)
{
    return scene
        ->finalPosShips[clickedPos.y() / SIZE_ONE_SELL + 1][clickedPos.x() / SIZE_ONE_SELL + 1];
}

void Battlefield::checkToKill(BattleScene *scene, int numberShotedShip)
{
    for (int i = 1; i < AMOUNT_CELLS_BY_WIDTH + 1; ++i) {
        for (int j = 1; j < AMOUNT_CELLS_BY_HEIGHT + 1; ++j) {
            if (scene->finalPosShips[i][j] == numberShotedShip) {
                return;
            }
        }
    }

    for (int i = 1; i < AMOUNT_CELLS_BY_WIDTH + 1; ++i) {
        for (int j = 1; j < AMOUNT_CELLS_BY_HEIGHT + 1; ++j) {
            if (scene->finalPosShips[i][j] == numberShotedShip + deltaShot) {
                QVector<QGraphicsItem *> itemsAtPoint = scene->items(QRectF((j - 1) * SIZE_ONE_SELL,
                                                                            (i - 1) * SIZE_ONE_SELL,
                                                                            SIZE_ONE_SELL,
                                                                            SIZE_ONE_SELL));
                for (QGraphicsItem *item : itemsAtPoint) {
                    QGraphicsPixmapItem *pixmapItem = dynamic_cast<QGraphicsPixmapItem *>(item);
                    if (pixmapItem && pixmapItem->zValue() == ZValueForClickOnScene) {
                        battleFirstScene->removeItem(pixmapItem);
                        delete pixmapItem;
                    }
                }
                setShipsBeforeTheDestroyed(scene,
                                           QPointF((j - 1) * SIZE_ONE_SELL,
                                                   (i - 1) * SIZE_ONE_SELL));

                QGraphicsPixmapItem *imageKill = new QGraphicsPixmapItem();
                imageKill->setZValue(ZValueForClickOnScene);
                imageKill->setPixmap(
                    killImage->scaled(SIZE_ONE_SELL, SIZE_ONE_SELL, Qt::KeepAspectRatio));
                imageKill->moveBy((j - 1) * SIZE_ONE_SELL, (i - 1) * SIZE_ONE_SELL);
                scene->addItem(imageKill);

                for (int k = i - 1; k < i + 2; ++k) {
                    for (int l = j - 1; l < j + 2; ++l) {
                        if (l > 0 && k > 0 && k < AMOUNT_CELLS_BY_WIDTH + 1
                            && l < AMOUNT_CELLS_BY_HEIGHT + 1 && scene->finalPosShips[k][l] == 0) {
                            QGraphicsPixmapItem *imageMiss = new QGraphicsPixmapItem();
                            imageMiss->setPixmap(missImage->scaled(SIZE_ONE_SELL,
                                                                   SIZE_ONE_SELL,
                                                                   Qt::KeepAspectRatio));
                            imageMiss->moveBy((l - 1) * SIZE_ONE_SELL, (k - 1) * SIZE_ONE_SELL);
                            scene->addItem(imageMiss);
                            scene->finalPosShips[k][l] += deltaShot;
                            if (playWithBot && scene == battleFirstScene) {
                                bot->field[k][l]++;
                            }
                        }
                    }
                }
                if (playWithBot && scene == battleFirstScene) {
                    bot->setBasic();
                }
            }
        }
    }
}

void Battlefield::botShoting()
{
    removeAimItem();
    delayTimerShot->stop();
    QGraphicsPixmapItem *image = new QGraphicsPixmapItem();
    image->setZValue(ZValueForClickOnScene);

    if (isHit(battleFirstScene, stepBot)) {
        handleBotHit(battleFirstScene, stepBot, image);
    } else {
        handleBotMiss(battleFirstScene, stepBot, image);
    }
    secondPlayerDidStep();
}

void Battlefield::removeAimItem()
{
    QVector<QGraphicsItem *> itemsAtPoint = battleFirstScene->items(
        QRectF(stepBot.x(), stepBot.y(), SIZE_ONE_SELL, SIZE_ONE_SELL));

    for (QGraphicsItem *item : itemsAtPoint) {
        QGraphicsPixmapItem *pixmapItem = dynamic_cast<QGraphicsPixmapItem *>(item);
        if (pixmapItem && pixmapItem->zValue() == ZValueForClickOnScene) {
            battleFirstScene->removeItem(pixmapItem);
            delete pixmapItem;
            break;
        }
    }
}

void Battlefield::handleBotHit(BattleScene *scene, QPointF clickedPos, QGraphicsPixmapItem *image)
{
    firstPlayerMissedShot();
    setImageProperties(image, hitImage, clickedPos);
    scene->addItem(image);
    incrementShotValue(scene, clickedPos);
    bot->setHitResult(true, clickedPos.x() / SIZE_ONE_SELL, clickedPos.y() / SIZE_ONE_SELL);
    checkToKill(scene, getShotValue(scene, clickedPos) - deltaShot);

    if (ui->lcdShip->value() != 0) {
        delayTimerGenerate->start(SPEED_AIM_TIMER);
    }
}

void Battlefield::handleBotMiss(BattleScene *scene, QPointF clickedPos, QGraphicsPixmapItem *image)
{
    setImageProperties(image, missImage, clickedPos);
    scene->addItem(image);
    firstPlayerIsShoting = true;
    ui->imageHand->setStyleSheet("image: url(:/image/image/rightHand.png)");
    incrementShotValue(scene, clickedPos);
    bot->setHitResult(false, clickedPos.x() / SIZE_ONE_SELL, clickedPos.y() / SIZE_ONE_SELL);
}

void Battlefield::checkFailedClick()
{
    if (!delayTimerShot->isActive() && !delayTimerGenerate->isActive()) {
        this->setEnabled(true);
    }

    if (isGameOver()) {
        showAllShips();
        (ui->lcdShip->value() == 0) ? StatsWindow::AddStats(false, playWithBot)
                                    : StatsWindow::AddStats(true, playWithBot);
        displayGameOverMessage();
        restartGame();
    }
}

bool Battlefield::isGameOver()
{
    return ui->lcdShip->value() == 0 || ui->lcdShip_2->value() == 0;
}

void Battlefield::displayGameOverMessage()
{
    (!ui->lcdShip->value()) ? ui->redWinLabel->setVisible(true)
                            : ui->greenWinLabel->setVisible(true);
    QMessageBox::information(
        this,
        (!ui->lcdShip->value()) ? "Победа " + ui->secondPlayerLabel->text()
                                : "Победа " + ui->firstPlayerLabel->text(),
        "При нажатии на любую из кнопок на этом окне программа будет перезапущена",
        QMessageBox::Ok,
        QMessageBox::Ok);
}

void Battlefield::restartGame()
{
    qApp->quit();
    QProcess::startDetached(qApp->arguments()[0], qApp->arguments());
}

void Battlefield::generateBotNumber()
{
    delayTimerGenerate->stop();
    stepBot = bot->makeMove();

    QGraphicsPixmapItem *image = new QGraphicsPixmapItem();
    image->setZValue(ZValueForClickOnScene);
    image->setPixmap(aimImage->scaled(SIZE_ONE_SELL, SIZE_ONE_SELL, Qt::KeepAspectRatio));
    image->moveBy(stepBot.x(), stepBot.y());
    battleFirstScene->addItem(image);

    delayTimerShot->start(SPEED_SHOT_TIMER);
}

void Battlefield::setShipsBeforeTheDestroyed(BattleScene *scene, QPointF point)
{
    QVector<QGraphicsItem *> shipsOnTheField = scene->items();
    for (QGraphicsItem *item : shipsOnTheField) {
        if (Ship *ship = dynamic_cast<Ship *>(item)) {
            if (ship->getStartPoint() == point) {
                ship->setVisible(true);
            }
        }
    }
}

void Battlefield::showAllShips()
{
    QVector<QGraphicsItem *> shipsOnTheField = battleFirstScene->items();
    for (QGraphicsItem *item : shipsOnTheField) {
        if (Ship *ship = dynamic_cast<Ship *>(item)) {
            ship->setVisible(true);
        }
    }

    shipsOnTheField = battleSecondScene->items();
    for (QGraphicsItem *item : shipsOnTheField) {
        if (Ship *ship = dynamic_cast<Ship *>(item)) {
            ship->setVisible(true);
        }
    }
}
