#ifndef ZOMBIE_H
#define ZOMBIE_H
#include <QPainter>
#include <QGraphicsItem>
#include <QObject>
#include <QPolygon>
#include <QPixmap>
#include <QTimer>
#include <QGraphicsScene>

class Zombie : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    explicit Zombie(QObject *parent = 0);
    QRectF boundingRect() const;
    void move(QPointF h);
    QPainterPath shape() const;
    void hit(int damage);
    int getHp() const;
    void setHp(int value);

public slots:
    void attack();
private:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    int hp=100;
    QPixmap zombie1={"Zombie-1a.png"};
    QTimer* timer;
    int animationstage=0;
    bool attackflag=0;
};

#endif // ZOMBIE_H
