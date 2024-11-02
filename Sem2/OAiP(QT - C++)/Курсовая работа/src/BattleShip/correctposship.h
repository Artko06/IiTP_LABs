#ifndef CORRECTPOSSHIP_H
#define CORRECTPOSSHIP_H

#include "buildbattlefield.h"

class CorrectPosShip
{
public:
    static void correctPosShip(BuildBattleField *setErrorOnField);
    static bool getErrorOneOfShips();
    static bool allShipsOnField();

private:
    static bool errorPosShip(int checkIndexI, int checkIndexJ);
    static void setErrorImageOnField(BuildBattleField *setErrorOnField, short numberShipError);
    static bool errorOneOfShips;
};

#endif // CORRECTPOSSHIP_H
