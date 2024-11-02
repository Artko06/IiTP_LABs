#ifndef FIELDFIRSTPLAYER_H
#define FIELDFIRSTPLAYER_H

#include "buildbattlefield.h"
#include "fieldsecondplayer.h"

namespace Ui {
class FieldFirstPlayer;
}

class FieldFirstPlayer : public QMainWindow
{
    Q_OBJECT

public:
    explicit FieldFirstPlayer(QWidget *parent = nullptr);
    ~FieldFirstPlayer();

signals:
    void backSignalFirstWindow();

public slots:
    void personButtonChoise();

    void botButtonChoise();

private slots:
    void on_backButton_clicked() {emit backSignalFirstWindow();};

    void on_startButton_clicked();

    void on_firstShipButton_clicked();

    void on_secondShipButton_clicked();

    void on_thirdShipButton_clicked();

    void on_forthShipButton_clicked();

    void searchErrorField();

    void on_autoButton_clicked();

private:
    Ui::FieldFirstPlayer *ui;
    FieldSecondPlayer* fieldSecondPlayer;
    Battlefield* battlefieldBot;
    BattleScene *sceneFirstPlayer, *sceneBot;
    BuildBattleField *buildBattleFieldPlayer1, *buildBattleFieldBot;
    QTimer *errorFieldTimer;

private:
    void hideShipsBeforeTheFight(BattleScene *scene);
};

#endif // FIELDFIRSTPLAYER_H
