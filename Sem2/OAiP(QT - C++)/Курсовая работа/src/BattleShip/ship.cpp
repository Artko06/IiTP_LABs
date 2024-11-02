#include "ship.h"

#include "ConstShips.h"

bool Ship::move = true;

Ship::Ship(int x, int y)
    : startPoint(QPointF(x, y))
{}

QPointF Ship::getStartPoint() const
{
    return this->startPoint;
}

QPointF Ship::getEndPoint() const
{
    return this->endPoint;
}

bool Ship::verticalRotateShip() const
{
    return !(static_cast<int>(this->rotation()) % FULL_TURN == 0);
}

bool Ship::isMove()
{
    return move;
}

void Ship::setStartPoint(const QPointF startPointValue)
{
    this->startPoint = startPointValue;
}

void Ship::setEndPoint(const QPointF endPointValue)
{
    this->endPoint = endPointValue;
}

void Ship::disMove()
{
    move = false;
}

void Ship::addShip(int x, int y, short typeShip, QString pathToFile)
{
    QPixmap pixmap(pathToFile);
    setPixmap(pixmap.scaled(typeShip * SIZE_ONE_SELL, SIZE_ONE_SELL, Qt::KeepAspectRatio));
    moveBy(x, y);
}

void Ship::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::RightButton && move) {
        if (static_cast<int>(this->rotation()) % FULL_TURN == 0) {
            this->setRotation(this->rotation() + CORNER_ROTATION);
        }

        else {
            this->setRotation(this->rotation() + 3 * CORNER_ROTATION);
        }
    }
    qDebug() << "click on item";
}

void Ship::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (move) {
        moveBy(event->scenePos().x() - event->lastScenePos().x(),
               event->scenePos().y() - event->lastScenePos().y());
    }
}
