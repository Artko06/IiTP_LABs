#ifndef SHIP_H
#define SHIP_H

#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPointF>

class Ship : public QGraphicsPixmapItem
{
public:
    Ship(int x, int y);
    QPointF getStartPoint() const;
    QPointF getEndPoint() const;
    bool verticalRotateShip() const;
    static bool isMove();
    void setStartPoint(const QPointF startPointValue);
    void setEndPoint(const QPointF endPointValue);
    static void disMove();

protected:
    void addShip(int x, int y, short typeShip, QString pathToFile);
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;

private:
    QPointF startPoint;
    QPointF endPoint;
    static bool move;
};

#endif // SHIP_H
