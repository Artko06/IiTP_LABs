#ifndef BOT_H
#define BOT_H

#include <QPointF>
#include <QRandomGenerator>
#include <QVector>

class Bot
{
public:
    Bot();
    QPointF makeMove();
    void setHitResult(bool hit, int x, int y);
    void setBasic();
    QVector<QVector<int>> field;

private:
    QPointF lastHit;
    QPoint firstHit;
    int direction = 0;
    int amountChangeDirection = 0;

private:
    QPointF randomShot();
    QPointF hitShip();
};

#endif // BOT_H
