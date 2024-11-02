#include "correctposship.h"

#include "ConstField.h"
#include "ConstShips.h"
#include "addships.h"

bool CorrectPosShip::errorOneOfShips;

void CorrectPosShip::correctPosShip(BuildBattleField *setErrorOnField)
{
    AddShips::arrayShips();
    errorOneOfShips = false;
    for (int i = 0; i < AMOUNT_CELLS_BY_WIDTH; ++i) {
        for (int j = 0; j < AMOUNT_CELLS_BY_HEIGHT; ++j) {
            setErrorOnField->getCellsVector()[i][j]->setBrush(Qt::NoBrush);
        }
    }

    for (int i = 0; i < AddShips::getListShips().size(); ++i) {
        for (int j = i + 1; j < AddShips::getListShips().size(); ++j) {
            if (AddShips::getListShips()[i]->getStartPoint()
                == AddShips::getListShips()[j]->getStartPoint()) {
                errorOneOfShips = true;
                setErrorOnField
                    ->getCellsVector()[AddShips::getListShips()[i]->getStartPoint().y()
                                       / SIZE_ONE_SELL]
                                      [AddShips::getListShips()[j]->getStartPoint().x()
                                       / SIZE_ONE_SELL]
                    ->setBrush(QImage(":/image/image/errorShips.jpg"));
            }
        }
    }

    for (int i = 1; i < AMOUNT_CELLS_BY_WIDTH + 2; ++i) {
        for (int j = 1; j < AMOUNT_CELLS_BY_HEIGHT + 2; ++j) {
            if (AddShips::getListPosShips()[i][j] != 0 && errorPosShip(i, j)) {
                errorOneOfShips = true;
                setErrorImageOnField(setErrorOnField, AddShips::getListPosShips()[i][j]);
            }
        }
    }
}

bool CorrectPosShip::getErrorOneOfShips()
{
    return errorOneOfShips;
}

bool CorrectPosShip::allShipsOnField()
{
    for (int k = 0; k < amountShips; ++k) {
        for (int i = 1; i < AMOUNT_CELLS_BY_WIDTH + 2; ++i) {
            for (int j = 1; j < AMOUNT_CELLS_BY_HEIGHT + 2; ++j) {
                if (AddShips::getListPosShips()[i][j] == k) {
                    ++k;
                    i = 0;
                    j = 0;
                }

                else if (i == AMOUNT_CELLS_BY_WIDTH + 1 && j == AMOUNT_CELLS_BY_HEIGHT + 1
                         && k < amountShips) {
                    return false;
                }
            }
        }
    }
    return true;
}

bool CorrectPosShip::errorPosShip(int checkIndexI, int checkIndexJ)
{
    for (int i = checkIndexI - 1; i < checkIndexI + 2; ++i) {
        for (int j = checkIndexJ - 1; j < checkIndexJ + 2; ++j) {
            if (AddShips::getListPosShips()[i][j] != 0
                && AddShips::getListPosShips()[i][j]
                       != AddShips::getListPosShips()[checkIndexI][checkIndexJ]) {
                return true;
            }
        }
    }
    return false;
}

void CorrectPosShip::setErrorImageOnField(BuildBattleField *setErrorOnField, short numberShipError)
{
    for (int i = 1; i < AMOUNT_CELLS_BY_WIDTH + 2; ++i) {
        for (int j = 1; j < AMOUNT_CELLS_BY_HEIGHT + 2; ++j) {
            if (AddShips::getListPosShips()[i][j] == numberShipError) {
                setErrorOnField->getCellsVector()[i - 1][j - 1]->setBrush(
                    QImage(":/image/image/errorShips.jpg"));
            }
        }
    }
}
