#ifndef SHIP2_H
#define SHIP2_H

#include "ship.h"

class Ship2 : public Ship
{
public:
    Ship2(int x, int y);

private:
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
};

#endif // SHIP2_H
