#ifndef BATTLEFIELD_H
#define BATTLEFIELD_H

#include <QLabel>
#include <QMainWindow>
#include <QPixmap>
#include <QTimer>
#include "battlescene.h"
#include "bot.h"
#include "buildbattlefield.h"

namespace Ui {
class Battlefield;
}

class Battlefield : public QMainWindow
{
    Q_OBJECT

public:
    explicit Battlefield(QWidget *parent = nullptr);
    ~Battlefield();

public:
    QLabel* getFirstLabel() const;
    QLabel* getSecondLabel() const;
    void setBattleFirstScene(BattleScene *scene);
    void setBattleSecondScene(BattleScene *scene);
    void goPlayWithBot();

signals:
    void botShouldDoingTheStep();

private slots:
    void on_menuButton_clicked();

    void onSceneClicked(BattleScene *scene, QPointF clickedPos);

    void generateBotNumber();

    void botShoting();

    void checkFailedClick();

private:
    QPixmap *missImage, *hitImage, *killImage, *aimImage;
    Ui::Battlefield *ui;
    BattleScene *battleFirstScene, *battleSecondScene;
    BuildBattleField *buildBattleFieldFirstScene, *buildBattleFieldSecondScene;
    bool firstPlayerIsShoting = true;

    Bot *bot;
    QTimer *delayTimerGenerate, *delayTimerShot, *checkFailClick;
    QPointF stepBot;
    bool playWithBot = false;

private:
    void handleFirstSceneClick(BattleScene *scene, QPointF clickedPos, QGraphicsPixmapItem *image);
    void handleSecondSceneClick(BattleScene *scene, QPointF clickedPos, QGraphicsPixmapItem *image);
    bool isShotValid(BattleScene *scene, QPointF clickedPos);
    bool isHit(BattleScene *scene, QPointF clickedPos);
    void handleHit(BattleScene *scene, QPointF clickedPos, QGraphicsPixmapItem *image);
    void handleMiss(BattleScene *scene, QPointF clickedPos, QGraphicsPixmapItem *image);
    void setImageProperties(QGraphicsPixmapItem *image, QPixmap *pixmap, QPointF clickedPos);
    void incrementShotValue(BattleScene *scene, QPointF clickedPos);
    int getShotValue(BattleScene *scene, QPointF clickedPos);
    void displayGameOverMessage();
    bool isGameOver();

    void checkToKill(BattleScene *scene, int numberShotedShip);
    void firstPlayerMissedShot();
    void secondPlayerMissedShot();
    void firstPlayerDidStep();
    void secondPlayerDidStep();

    void removeAimItem();
    void handleBotHit(BattleScene *scene, QPointF clickedPos, QGraphicsPixmapItem *image);
    void handleBotMiss(BattleScene *scene, QPointF clickedPos, QGraphicsPixmapItem *image);
    void restartGame();
    void setShipsBeforeTheDestroyed(BattleScene *scene, QPointF point);

    void showAllShips();
};

#endif // BATTLEFIELD_H
