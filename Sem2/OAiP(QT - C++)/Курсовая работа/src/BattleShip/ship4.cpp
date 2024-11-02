#include "ship4.h"

#include "ConstField.h"
#include "ConstShips.h"
#include "selectTypeShip.h"

Ship4::Ship4(int x, int y)
    : Ship(x, y)
{
    addShip(x, y, static_cast<int>(TypeShip::ForthShip), ":/image/image/forthShip.png");
    setTransformOriginPoint(boundingRect().center()
                            - QPointF(3 * BLEND_X_FOR_ROTATION, BLEND_Y_FOR_ROTATION));
    setEndPoint(QPoint(x + this->boundingRect().width(), y + this->boundingRect().height()));
    setZValue(FIRST_PLACE_ON_SCENE);
}

void Ship4::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
    QPointF newPos = QPointF(qRound(this->pos().x() / SIZE_ONE_SELL) * SIZE_ONE_SELL,
                             qRound(this->pos().y() / SIZE_ONE_SELL) * SIZE_ONE_SELL);

    if (newPos.x() < LOWER_LIMIT_X_FOR_FORTH_SHIPS) {
        newPos.setX(LOWER_LIMIT_X_FOR_FORTH_SHIPS);
    }

    if (newPos.y() < LOWER_LIMIT_Y_FOR_FORTH_SHIPS) {
        newPos.setY(LOWER_LIMIT_Y_FOR_FORTH_SHIPS);
    }

    if (static_cast<int>(this->rotation()) % FULL_TURN != 0) {
        if (newPos.x() > UPPER_LIMIT_Y_FOR_FORTH_SHIPS) {
            newPos.setX(UPPER_LIMIT_Y_FOR_FORTH_SHIPS);
        }

        if (newPos.y() > UPPER_LIMIT_X_FOR_FORTH_SHIPS) {
            newPos.setY(UPPER_LIMIT_X_FOR_FORTH_SHIPS);
        }

        setEndPoint(newPos + QPointF(this->boundingRect().height(), this->boundingRect().width()));
    }

    else {
        if (newPos.x() > UPPER_LIMIT_X_FOR_FORTH_SHIPS) {
            newPos.setX(UPPER_LIMIT_X_FOR_FORTH_SHIPS);
        }

        if (newPos.y() > UPPER_LIMIT_Y_FOR_FORTH_SHIPS) {
            newPos.setY(UPPER_LIMIT_Y_FOR_FORTH_SHIPS);
        }

        setEndPoint(newPos + QPointF(this->boundingRect().width(), this->boundingRect().height()));
    }

    setStartPoint(newPos);
    setPos(newPos);
}
