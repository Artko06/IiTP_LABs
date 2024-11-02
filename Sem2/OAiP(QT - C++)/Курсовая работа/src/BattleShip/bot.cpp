#include "bot.h"

#include "ConstBot.h"
#include "ConstField.h"
#include "ConstShips.h"

Bot::Bot()
    : lastHit(nothing, nothing)
    , firstHit(nothing, nothing)
{
    field.fill(QVector<int>(AMOUNT_CELLS_BY_WIDTH + 2, emptyField), AMOUNT_CELLS_BY_HEIGHT + 2);
}

QPointF Bot::makeMove()
{
    if (lastHit.x() != nothing && lastHit.y() != nothing) {
        return hitShip();
    } else {
        return randomShot();
    }
}

void Bot::setHitResult(bool hit, int x, int y)
{
    field[y + 1][x + 1] = hit ? deltaShot : missShot;
    if (hit) {
        if (firstHit.x() == nothing && firstHit.y() == nothing) {
            firstHit = QPoint(x, y);
        }
        lastHit = QPointF(x, y);
    } else if (firstHit.x() != nothing && firstHit.y() != nothing) {
        lastHit = firstHit;
        direction = (direction + 1) % allFourSide;
    }
}

void Bot::setBasic()
{
    firstHit = QPoint(nothing, nothing);
    lastHit = QPointF(nothing, nothing);
    direction = 0;
}

QPointF Bot::randomShot()
{
    int x = 0, y = 0;
    do {
        x = QRandomGenerator::global()->bounded(amountShips);
        y = QRandomGenerator::global()->bounded(amountShips);
    } while (field[y + 1][x + 1] != emptyField);
    return QPointF(x * SIZE_ONE_SELL, y * SIZE_ONE_SELL);
}

QPointF Bot::hitShip()
{
    QPointF nextShot = (amountChangeDirection > allFourSide) ? firstHit : lastHit;
    switch (direction) {
    case LEFT_SIDE:
        nextShot.rx()--;
        break;
    case DOWN_SIDE:
        nextShot.ry()++;
        break;
    case RIGHT_SIDE:
        nextShot.rx()++;
        break;
    case UPP_SIDE:
        nextShot.ry()--;
        break;
    }
    if (nextShot.x() < 0 || nextShot.x() >= amountShips || nextShot.y() < 0
        || nextShot.y() >= amountShips || field[nextShot.y() + 1][nextShot.x() + 1] != emptyField) {
        direction = (direction + 1) % allFourSide;
        ++amountChangeDirection;
        return hitShip();
    }
    amountChangeDirection = 0;
    return QPointF(nextShot.x() * SIZE_ONE_SELL, nextShot.y() * SIZE_ONE_SELL);
}
