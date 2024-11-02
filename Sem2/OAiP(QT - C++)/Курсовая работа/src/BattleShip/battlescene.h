#ifndef BATTLESCENE_H
#define BATTLESCENE_H

#include <QGraphicsScene>
#include <ship.h>

class BattleScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit BattleScene(QObject *parent = nullptr);
    QVector<QVector<int>> finalPosShips;
    QVector<Ship *> listShips;
    void convertToPos();

signals:
    void sceneClicked(BattleScene *scene, QPointF pointClicked);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
};

#endif // BATTLESCENE_H
