#ifndef ZOMBIE_H
#define ZOMBIE_H
#include <QPainter>
#include <QGraphicsItem>
#include <QObject>
#include <QPolygon>
#include <QPixmap>
#include <QTimer>
#include <QGraphicsScene>
#include <QSound>

class Zombie : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    explicit Zombie(QObject *parent = 0);
    ~Zombie();
    QRectF boundingRect() const;
    void move(QPointF h);
    QPainterPath shape() const;
    void hit(int damage);
    int getHp() const;
    void setHp(int value);

    int getAnimationstage() const;
    void setAnimationstage(int value);

    int getTime() const;
    void setTime(int value);

public slots:
    void attack();
    void del();
private:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    int hp=100;
    float speed=1.5;
    QPixmap zombie1={"Zombie-1a.png"};
    QTimer* timer;
    int animationstage=0;
    bool attackflag=0;
    int time=0;
};

#endif // ZOMBIE_H
