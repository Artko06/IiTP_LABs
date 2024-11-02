#ifndef SHIP3_H
#define SHIP3_H

#include "ship.h"

class Ship3 : public Ship
{
public:
    Ship3(int x, int y);

private:
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
};

#endif // SHIP3_H
