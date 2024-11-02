#ifndef SHIP4_H
#define SHIP4_H

#include "ship.h"

class Ship4 : public Ship
{
public:
    Ship4(int x, int y);

private:
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
};

#endif // SHIP4_H
