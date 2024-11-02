#include "addships.h"

#include <QRandomGenerator>
#include "ConstAutoPlacing.h"
#include "ConstField.h"
#include "ConstShips.h"
#include "ship1.h"
#include "ship2.h"
#include "ship3.h"
#include "ship4.h"

QVector<QVector<int>> AddShips::arrShips(amountShips + 2, QVector<int>(amountShips + 2));
QVector<QVector<int>> AddShips::autoPlacingMatrix(amountShips + 2,
                                                  QVector<int>(amountShips + 2, FREE_CELLS));
QVector<Ship *> AddShips::listShips;

void AddShips::addShipsToScene(int posX, int posY, TypeShip typeShip, QGraphicsScene *scene)
{
    switch (typeShip) {
    case TypeShip::FirstShip: {
        Ship1 *ship = new Ship1(posX, posY);
        listShips.append(ship);
        scene->addItem(ship);
        break;
    }
    case TypeShip::SecondShip: {
        Ship2 *ship = new Ship2(posX, posY);
        listShips.append(ship);
        scene->addItem(ship);
        break;
    }
    case TypeShip::ThirdShip: {
        Ship3 *ship = new Ship3(posX, posY);
        listShips.append(ship);
        scene->addItem(ship);
        break;
    }
    case TypeShip::ForthShip: {
        Ship4 *ship = new Ship4(posX, posY);
        listShips.append(ship);
        scene->addItem(ship);
        break;
    }
    }
}

QVector<QVector<int>> &AddShips::getListPosShips()
{
    return arrShips;
}

QVector<Ship *> &AddShips::getListShips()
{
    return listShips;
}

void AddShips::arrayShips()
{
    for (int i = 0; i < amountShips + 2; ++i) {
        for (int j = 0; j < amountShips + 2; ++j) {
            arrShips[i][j] = 0;
        }
    }

    for (int i = 0; i < listShips.size(); ++i) {
        if (listShips.at(i)->verticalRotateShip()) {
            for (int start = listShips.at(i)->getStartPoint().y() / SIZE_ONE_SELL;
                 start < listShips.at(i)->getEndPoint().y() / SIZE_ONE_SELL;
                 ++start) {
                arrShips[start + 1][listShips.at(i)->getStartPoint().x() / SIZE_ONE_SELL + 1] = i
                                                                                                + 1;
            }
        }

        else {
            for (int start = listShips.at(i)->getStartPoint().x() / SIZE_ONE_SELL;
                 start < listShips.at(i)->getEndPoint().x() / SIZE_ONE_SELL;
                 ++start) {
                arrShips[listShips.at(i)->getStartPoint().y() / SIZE_ONE_SELL + 1][start + 1] = i
                                                                                                + 1;
            }
        }
    }
}

void AddShips::autoPlacing(QGraphicsScene *scene)
{
    deleteAllShips(scene);
    QPointF startPosShip;
    bool horizontalPos = true;

    for (int i = 0; i < AMOUNT_CELLS_BY_WIDTH + 2; ++i) {
        for (int j = 0; j < AMOUNT_CELLS_BY_HEIGHT + 2; ++j) {
            autoPlacingMatrix[i][j] = FREE_CELLS;
        }
    }

    for (int i = 0; i < amountForthShips;) {
        horizontalPos = QRandomGenerator::global()->bounded(2);
        startPosShip = generatorPoint(AMOUNT_GENERATED_NUMBERS_FOR_FORTH_SHIPS);
        if (addShipForAutoPlacing(startPosShip.x(),
                                  startPosShip.y(),
                                  TypeShip::ForthShip,
                                  horizontalPos)) {
            listShips.append(new Ship4(startPosShip.x(), startPosShip.y()));
            if (!horizontalPos) {
                rotateToVecticalPos(listShips.at(i));
                listShips.at(i)->setEndPoint(
                    QPointF(listShips.at(i)->getEndPoint().x() - 3 * SIZE_ONE_SELL,
                            listShips.at(i)->getEndPoint().y() + 3 * SIZE_ONE_SELL));
            }
            scene->addItem(listShips.at(i));
            ++i;
        }
    }

    for (int i = amountForthShips; i < amountThirdShips + amountForthShips;) {
        horizontalPos = QRandomGenerator::global()->bounded(2);
        startPosShip = generatorPoint(AMOUNT_GENERATED_NUMBERS_FOR_THIRD_SHIPS);
        if (addShipForAutoPlacing(startPosShip.x(),
                                  startPosShip.y(),
                                  TypeShip::ThirdShip,
                                  horizontalPos)) {
            listShips.append(new Ship3(startPosShip.x(), startPosShip.y()));
            if (!horizontalPos) {
                rotateToVecticalPos(listShips.at(i));
                listShips.at(i)->setEndPoint(
                    QPointF(listShips.at(i)->getEndPoint().x() - 2 * SIZE_ONE_SELL,
                            listShips.at(i)->getEndPoint().y() + 2 * SIZE_ONE_SELL));
            }
            scene->addItem(listShips.at(i));
            ++i;
        }
    }

    for (int i = amountThirdShips + amountForthShips;
         i < amountSecondShips + amountThirdShips + amountForthShips;) {
        horizontalPos = QRandomGenerator::global()->bounded(2);
        startPosShip = generatorPoint(AMOUNT_GENERATED_NUMBERS_FOR_SECOND_SHIPS);
        if (addShipForAutoPlacing(startPosShip.x(),
                                  startPosShip.y(),
                                  TypeShip::SecondShip,
                                  horizontalPos)) {
            listShips.append(new Ship2(startPosShip.x(), startPosShip.y()));
            if (!horizontalPos) {
                rotateToVecticalPos(listShips.at(i));
                listShips.at(i)->setEndPoint(
                    QPointF(listShips.at(i)->getEndPoint().x() - SIZE_ONE_SELL,
                            listShips.at(i)->getEndPoint().y() + SIZE_ONE_SELL));
            }
            scene->addItem(listShips.at(i));
            ++i;
        }
    }

    for (int i = amountSecondShips + amountThirdShips + amountForthShips;
         i < amountFirstShips + amountSecondShips + amountThirdShips + amountForthShips;) {
        horizontalPos = QRandomGenerator::global()->bounded(2);
        startPosShip = generatorPoint(AMOUNT_GENERATED_NUMBERS_FOR_FIRST_SHIPS);
        if (addShipForAutoPlacing(startPosShip.x(),
                                  startPosShip.y(),
                                  TypeShip::FirstShip,
                                  horizontalPos)) {
            listShips.append(new Ship1(startPosShip.x(), startPosShip.y()));
            if (!horizontalPos) {
                rotateToVecticalPos(listShips.at(i));
            }
            scene->addItem(listShips.at(i));
            ++i;
        }
    }
}

void AddShips::deleteListOfShips()
{
    listShips.resize(0);
}

void AddShips::rotateToVecticalPos(Ship *ship)
{
    ship->setRotation(ship->rotation() + CORNER_ROTATION);
}

bool AddShips::checkShipForAutoPlacing(int posX, int posY, TypeShip typeShip, bool horizontalPos)
{
    if (horizontalPos) {
        for (int i = posX / SIZE_ONE_SELL + 1;
             i < posX / SIZE_ONE_SELL + 1 + static_cast<int>(typeShip);
             ++i) {
            if (autoPlacingMatrix[posY / SIZE_ONE_SELL + 1][i] == OCCUPIED_CELLS) {
                return false;
            }
        }

        for (int i = posY / SIZE_ONE_SELL; i < posY / SIZE_ONE_SELL + 3; ++i) {
            for (int j = posX / SIZE_ONE_SELL;
                 j < posX / SIZE_ONE_SELL + 2 + static_cast<int>(typeShip);
                 ++j) {
                autoPlacingMatrix[i][j] = OCCUPIED_CELLS;
            }
        }
    }

    else {
        for (int i = posY / SIZE_ONE_SELL + 1;
             i < posY / SIZE_ONE_SELL + 1 + static_cast<int>(typeShip);
             ++i) {
            if (autoPlacingMatrix[i][posX / SIZE_ONE_SELL + 1] == OCCUPIED_CELLS) {
                return false;
            }
        }

        for (int i = posX / SIZE_ONE_SELL; i < posX / SIZE_ONE_SELL + 3; ++i) {
            for (int j = posY / SIZE_ONE_SELL;
                 j < posY / SIZE_ONE_SELL + 2 + static_cast<int>(typeShip);
                 ++j) {
                autoPlacingMatrix[j][i] = OCCUPIED_CELLS;
            }
        }
    }
    return true;
}

bool AddShips::addShipForAutoPlacing(int posX, int posY, TypeShip typeShip, bool horizontalPos)
{
    switch (typeShip) {
    case TypeShip::FirstShip: {
        return checkShipForAutoPlacing(posX, posY, TypeShip::FirstShip, horizontalPos);
    }
    case TypeShip::SecondShip: {
        return checkShipForAutoPlacing(posX, posY, TypeShip::SecondShip, horizontalPos);
    }
    case TypeShip::ThirdShip: {
        return checkShipForAutoPlacing(posX, posY, TypeShip::ThirdShip, horizontalPos);
    }
    case TypeShip::ForthShip: {
        return checkShipForAutoPlacing(posX, posY, TypeShip::ForthShip, horizontalPos);
    }
    }
    return true;
}

QPointF AddShips::generatorPoint(int amountGenerate)
{
    return QPointF(static_cast<int>(QRandomGenerator::global()->bounded(amountGenerate)
                                    / SIZE_ONE_SELL)
                       * SIZE_ONE_SELL,
                   static_cast<int>(QRandomGenerator::global()->bounded(amountGenerate)
                                    / SIZE_ONE_SELL)
                       * SIZE_ONE_SELL);
}

void AddShips::deleteAllShips(QGraphicsScene *scene)
{
    listShips.resize(0);
    QVector<QGraphicsItem *> foundItems = scene->items();
    for (QGraphicsItem *item : foundItems) {
        if (Ship *ship = dynamic_cast<Ship *>(item)) {
            scene->removeItem(ship);
            delete ship;
        }
    }
}
