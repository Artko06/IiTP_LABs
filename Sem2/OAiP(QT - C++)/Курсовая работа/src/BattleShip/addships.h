#ifndef ADDSHIPS_H
#define ADDSHIPS_H

#include <QGraphicsScene>
#include <QVector>
#include "selectTypeShip.h"
#include "ship.h"

class AddShips
{
public:
    static void addShipsToScene(int posX, int posY, TypeShip typeShip, QGraphicsScene *scene);
    static QVector<QVector<int>> &getListPosShips();
    static QVector<Ship *> &getListShips();
    static void arrayShips();
    static void autoPlacing(QGraphicsScene *scene);
    static void deleteListOfShips();

private:
    static void rotateToVecticalPos(Ship *ship);
    static bool checkShipForAutoPlacing(int posX, int posY, TypeShip typeShip, bool horizontalPos);
    static bool addShipForAutoPlacing(int posX, int posY, TypeShip typeShip, bool horizontalPos);
    static QVector<QVector<int>> autoPlacingMatrix;
    static QVector<QVector<int>> arrShips;
    static QVector<Ship *> listShips;
    static QPointF generatorPoint(int amountGenerate);
    static void deleteAllShips(QGraphicsScene *scene);
};

#endif // ADDSHIPS_H
