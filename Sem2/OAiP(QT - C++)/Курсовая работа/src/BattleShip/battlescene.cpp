#include "battlescene.h"
#include "qgraphicssceneevent.h"

#include <QGraphicsItem>
#include "ConstShips.h"

BattleScene::BattleScene(QObject *parent)
    : QGraphicsScene(parent)
    , finalPosShips(amountShips + 2, QVector<int>(amountShips + 2, 0))
    , listShips(amountShips)
{}

void BattleScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (!Ship::isMove() && event->button() == Qt::LeftButton) {
        QPointF clickPos = QPointF(static_cast<int>(event->scenePos().x() / SIZE_ONE_SELL)
                                       * SIZE_ONE_SELL,
                                   static_cast<int>(event->scenePos().y() / SIZE_ONE_SELL)
                                       * SIZE_ONE_SELL);
        emit sceneClicked(this, clickPos);
    }

    else {
        QGraphicsScene::mousePressEvent(event);
    }
}

void BattleScene::convertToPos()
{
    for (int i = 0; i < amountShips; ++i) {
        for (int j = listShips[i]->getStartPoint().x();
             j <= listShips[i]->getEndPoint().x() - SIZE_ONE_SELL;
             j += SIZE_ONE_SELL) {
            for (int k = listShips[i]->getStartPoint().y();
                 k <= listShips[i]->getEndPoint().y() - SIZE_ONE_SELL;
                 k += SIZE_ONE_SELL) {
                finalPosShips[k / SIZE_ONE_SELL + 1][j / SIZE_ONE_SELL + 1] = i + 1;
            }
        }
    }
}
