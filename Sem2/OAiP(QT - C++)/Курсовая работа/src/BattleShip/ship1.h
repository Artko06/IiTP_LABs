#ifndef SHIP1_H
#define SHIP1_H

#include "ship.h"

class Ship1 : public Ship
{
public:
    Ship1(int x, int y);

private:
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
};

#endif // SHIP1_H
