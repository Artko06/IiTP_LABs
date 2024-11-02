#ifndef FIELDSECONDPLAYER_H
#define FIELDSECONDPLAYER_H

#include <QString>
#include "battlefield.h"
#include "buildbattlefield.h"

namespace Ui {
class FieldSecondPlayer;
}

class FieldSecondPlayer : public QMainWindow
{
    Q_OBJECT

public:
    explicit FieldSecondPlayer(QWidget *parent = nullptr);
    ~FieldSecondPlayer();
    void setNicknamePerson1(const QString nicknamePerson1);
    void setSceneFirstPlayerOnSecondWindow(BattleScene *scene);

signals:
    void backSignalSecondWindow();

private slots:
    void on_startButton_clicked();

    void on_menuButton_clicked();

    void searchErrorField();

    void on_autoButton_clicked();

    void on_firstShipButton_clicked();

    void on_secondShipButton_clicked();

    void on_thirdShipButton_clicked();

    void on_forthShipButton_clicked();

private:
    Ui::FieldSecondPlayer *ui;
    Battlefield* battlefieldPerson;
    BattleScene *sceneFirstPlayerOnSecondWindow = nullptr;
    BattleScene *sceneSecondPlayer;
    QString nicknamePerson1;
    BuildBattleField *buildBattleFieldPlayer2;
    QTimer *errorFieldTimer;

private:
    void hideShipsBeforeTheFight(BattleScene *scene);
};

#endif // FIELDSECONDPLAYER_H
