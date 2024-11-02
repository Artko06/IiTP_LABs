#ifndef BUILDBATTLEFIELD_H
#define BUILDBATTLEFIELD_H

#include <QGraphicsScene>
#include <QVector>

class BuildBattleField
{
public:
    explicit BuildBattleField(QGraphicsScene *scene);
    QVector<QVector<QGraphicsRectItem *>> getCellsVector();

private:
    QVector<QVector<QGraphicsRectItem *>> listCells;
};

#endif // BUILDBATTLEFIELD_H
