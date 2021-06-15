#include "zombie.h"
#include <QPainter>
#include <QDebug>
#include <QGraphicsItem>
#include <QElapsedTimer>
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
    setCacheMode(QGraphicsItem::ItemCoordinateCache);
}

void Zombie::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if (hp<=0)
        painter->drawPixmap(-75,-75,150,150,QPixmap("deadzombie-1.png"));
    else{
        switch (animationstage) {
        case 0:
            painter->drawPixmap(-50,-50,100,100,zombie1,0,0,100,100);
            break;
        case 1:
            painter->drawPixmap(-50,-50,100,100,zombie1,100,0,100,100);
            break;
        case 2:
             painter->drawPixmap(-50,-50,100,100,zombie1,200,0,100,100);
            break;
        case 3:
             painter->drawPixmap(-50,-50,100,100,zombie1,300,0,100,100);
            break;
        case 4:
             painter->drawPixmap(-50,-50,100,100,zombie1,400,0,100,100);
            break;
        case 5:
             painter->drawPixmap(-50,-50,100,100,zombie1,500,0,100,100);
            break;
        case 6:
             painter->drawPixmap(-50,-50,100,100,zombie1,600,0,100,100);
            break;
        default:
            painter->drawPixmap(-50,-50,100,100,zombie1,0,0,100,100);
            break;
        }
    }


}

int Zombie::getHp() const
{
    return hp;
}

void Zombie::setHp(int value)
{
    hp = value;
}

QRectF Zombie::boundingRect() const
{
    if (hp<=0)
        return QRectF(-75,-75,150,150);
    else
        return QRectF(-50,-50,100,100);   // Ограничиваем область, в которой лежит
}

void Zombie::move(QPointF h)
{
    if (hp>0 && !attackflag){
        if ((h.x()-this->x())*(h.x()-this->x())+(h.y()-this->y())*(h.y()-this->y())<=14400){
            timer=new QTimer(this);
            connect(timer,SIGNAL(timeout()),this,SLOT(attack()));
            this->attack();
            timer->start(100);
            attackflag=1;
        }
        else{
            if(this->x()<h.x()){
                this->setX(this->x()+1);
                QPoint a(this->x()+50,this->y()+50);
                for (int i=0; i<scene()->items().size(); i++)
                {
                    QPoint b(scene()->items().at(i)->x()+50,scene()->items().at(i)->y()+50);
                    if ((a.x()-b.x())*(a.x()-b.x())+(a.y()-b.y())*(a.y()-b.y())<=4*50*50 &&   scene()->items().at(i)!=this && (dynamic_cast <Zombie*> (scene()->items().at(i)))){
                        if (dynamic_cast<Zombie*> (scene()->items().at(i))->getHp()>0)
                        this->setX(this->x() - 1);
                        break;
                    }
                }

            }
            if(this->x()>h.x()){
                this->setX(this->x()-1);
                QPoint a(this->x()+50,this->y()+50);
                for (int i=0; i<scene()->items().size(); i++)
                {
                    QPoint b(scene()->items().at(i)->x()+50,scene()->items().at(i)->y()+50);
                    if (    (a.x()-b.x())*(a.x()-b.x())+(a.y()-b.y())*(a.y()-b.y())<=4*50*50 &&   scene()->items().at(i)!=this && (dynamic_cast<Zombie*> (scene()->items().at(i)))){
                        if (dynamic_cast<Zombie*> (scene()->items().at(i))->getHp()>0)
                        this->setX(this->x() + 1);
                        break;
                    }
                }
            }
            if(this->y()<h.y()){
                this->setY(this->y()+1);
                QPoint a(this->x()+50,this->y()+50);
                for (int i=0; i<scene()->items().size(); i++)
                {
                    QPoint b(scene()->items().at(i)->x()+50,scene()->items().at(i)->y()+50);
                    if (    (a.x()-b.x())*(a.x()-b.x())+(a.y()-b.y())*(a.y()-b.y())<=4*50*50 &&   scene()->items().at(i)!=this && (dynamic_cast<Zombie*> (scene()->items().at(i)))){
                        if (dynamic_cast<Zombie*> (scene()->items().at(i))->getHp()>0)
                            this->setY(this->y() - 1);
                        break;
                    }
                }
            }
            if(this->y()>h.y()){
                this->setY(this->y()-1);
                QPoint a(this->x()+50,this->y()+50);
                for (int i=0; i<scene()->items().size(); i++)
                {
                    QPoint b(scene()->items().at(i)->x()+50,scene()->items().at(i)->y()+50);
                    if (    (a.x()-b.x())*(a.x()-b.x())+(a.y()-b.y())*(a.y()-b.y())<=4*50*50 &&   scene()->items().at(i)!=this && (dynamic_cast<Zombie*> (scene()->items().at(i)))){
                        if (dynamic_cast<Zombie*> (scene()->items().at(i))->getHp()>0)
                        this->setY(this->y() + 1);
                        break;
                    }

                }
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
        path.addRect(boundingRect());
        return path;
    }
    else
        return path;
}

void Zombie::hit(int damage)
{
    hp -= damage;
    qDebug()<<hp;
    this->update();

}

void Zombie::attack()
{
    animationstage++;
    if(animationstage>8){
        attackflag=0;
        animationstage=0;
        timer->stop();
    }
    this->update();
}

