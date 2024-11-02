#include "buildbattlefield.h"

#include <QGraphicsRectItem>
#include "ConstField.h"
#include "ConstShips.h"

BuildBattleField::BuildBattleField(QGraphicsScene *scene)
    : listCells(AMOUNT_CELLS_BY_WIDTH, QVector<QGraphicsRectItem *>(AMOUNT_CELLS_BY_HEIGHT))
{
    for (int i = 0; i < AMOUNT_CELLS_BY_WIDTH; ++i) {
        for (int j = 0; j < AMOUNT_CELLS_BY_HEIGHT; ++j) {
            QGraphicsRectItem *cell = new QGraphicsRectItem(i * SIZE_ONE_SELL,
                                                            j * SIZE_ONE_SELL,
                                                            SIZE_ONE_SELL,
                                                            SIZE_ONE_SELL);
            cell->setPen(QPen(Qt::blue));
            scene->addItem(cell);
            listCells[j][i] = cell;
        }
    }
}

QVector<QVector<QGraphicsRectItem *> > BuildBattleField::getCellsVector()
{
    return listCells;
}
