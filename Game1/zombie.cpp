#include "zombie.h"
#include <QPainter>
#include <QDebug>
#include <QGraphicsItem>
#include <math.h>


static const double Pi = 3.14159265358979323846264338327950288419717;
static double TwoPi = 2.0 * Pi;

static qreal normalizeAngle(qreal angle)
{
    while (angle < 0)
        angle += TwoPi;
    while (angle > TwoPi)
        angle -= TwoPi;
    return angle;
}


Zombie::Zombie(QObject *parent)
{

}

void Zombie::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if (hp<=0)
        painter->drawPixmap(-75,-75,150,150,QPixmap("deadzombie-1.png"));
    else
        painter->drawPixmap(-50,-50,100,100,QPixmap("Zombie-1.png"));
}

QRectF Zombie::boundingRect() const
{
    if (hp<=0)
        return QRectF(-150,-150,300,300);
    else
        return QRectF(-50,-50,100,100);   // Ограничиваем область, в которой лежит
}

void Zombie::move(QPointF h)
{
    if (hp>0){
        if (sqrt((h.x()-this->x())*(h.x()-this->x())+(h.y()-this->y())*(h.y()-this->y()))<=120){
        }
        else{
            if(this->x()<h.x()){
                this->setX(this->x()+1);
                if(!scene()->collidingItems(this).isEmpty())
                            this->setX(this->x() - 3);
            }
            if(this->x()>h.x()){
                this->setX(this->x()-1);
                if(!scene()->collidingItems(this).isEmpty())
                            this->setX(this->x() + 3);
            }
            if(this->y()<h.y()){
                this->setY(this->y()+1);
                if(!scene()->collidingItems(this).isEmpty())
                            this->setY(this->y() - 3);
            }
            if(this->y()>h.y()){
                this->setY(this->y()-1);
                if(!scene()->collidingItems(this).isEmpty())
                            this->setY(this->y() + 3);
            }

            QLineF lineToTarget(QPointF(0, 0), mapFromScene(h));
            // Угол поворота в направлении к цели
            qreal angleToTarget = ::acos(lineToTarget.dx() / lineToTarget.length());
            if (lineToTarget.dy() < 0)
                angleToTarget = TwoPi - angleToTarget;
            angleToTarget = normalizeAngle((Pi - angleToTarget) + Pi / 2);

            /* В Зависимости от того, слева или справа находится Цель от Героя,
             * устанавливаем направление поворота Героя в данном тике таймера
             * */
            if (angleToTarget >= 0 && angleToTarget < Pi) {
                // Rotate left
                setRotation(rotation() - angleToTarget * 180 /Pi);
            } else if (angleToTarget <= TwoPi && angleToTarget > Pi) {
                // Rotate right
                setRotation(rotation() + (angleToTarget - TwoPi )* (-180) /Pi);
            }
        }
    }
}

QPainterPath Zombie::shape() const
{
    QPainterPath path;
    if (hp>0){
        path.addEllipse(boundingRect());
        return path;
    }
    else
        return path;
}

void Zombie::hit(int damage)
{
    hp -= damage;   // Уменьшаем здоровье мишени
    qDebug()<<hp;
    this->update();

}

