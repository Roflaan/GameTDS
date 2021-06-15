#include "human.h"
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

Human::Human(QObject *parent)
{
    setRotation(0);
}




QRectF Human::boundingRect() const
{
    return QRectF(-50,-50,100,100);   // Ограничиваем область, в которой лежит
}




void Human::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{

//    QPolygon polygon;   // Используем класс полигона, чтобы отрисовать треугольник
//    // Помещаем координаты точек в полигональную модель
//    polygon << QPoint(0,-40) << QPoint(25,40) << QPoint(-25,40);
//    painter->setBrush(Qt::red);     // Устанавливаем кисть, которой будем отрисовывать объект
    if (shotEffect)
        painter->drawPixmap(-50,-50,100,100,QPixmap("shot.png"));
    else
        painter->drawPixmap(-50,-50,100,100,QPixmap("mc.png"));
//    painter->drawRect(-50,-50,100,100);
//    Q_UNUSED(option);
    //    Q_UNUSED(widget);
}



void Human::slotTarget(QPointF point)
{
    // Определяем расстояние до цели
    target = point;
    QLineF lineToTarget(QPointF(0, 0), mapFromScene(target));
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





void Human::slotGameTimer()
{
    if(GetAsyncKeyState('A') && this->x()>1){
        this->setX(this->x() - speed);
        target.rx()-=speed;
    }
    if(GetAsyncKeyState('D') && this->x()<2999){
        this->setX(this->x() + speed);
        target.rx()+=speed;
    }
    if(GetAsyncKeyState('W') && this->y()>1){
        this->setY(this->y() - speed);
        target.ry()-=speed;
    }
    if(GetAsyncKeyState('S')&& this->y()<2999){
        this->setY(this->y() + speed);
        target.ry()+=speed;
    }


    QLineF lineToTarget(QPointF(0, 0), mapFromScene(target));
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
    emit signalPlayerCoordinate(QPointF (this->x(),this->y()));



}

int Human::getMoney() const
{
    return money;
}

void Human::setMoney(int value)
{
    money = value;
}

int Human::getAmmoSum() const
{
    return ammoSum;
}

void Human::setAmmoSum(int value)
{
    ammoSum = value;
}

int Human::getHp() const
{
    return hp;
}

void Human::setHp(int value)
{
    hp = value;
}

int Human::getAmmoP() const
{
    return ammoP;
}

void Human::setAmmoP(int value)
{
    ammoP = value;
}

void Human::shot()
{
    shotEffect=true;
    allowShoot=false;
    fireTimer = new QTimer();
    connect(fireTimer, &QTimer::timeout, this, &Human::shotEnd);
    this->update();
    fireTimer->start(70);
}

void Human::shotEnd()
{
    static int num=0;
    if (num==7){
        delete fireTimer;
        allowShoot=true;
        num=0;
    }
    else{
        shotEffect=false;
        num+=1;
    }
}

bool Human::getAllowshoot() const
{
    return allowShoot;
}

void Human::setAllowshoot(bool value)
{
    allowShoot = value;
}

bool Human::getFire() const
{
    return shotEffect;
}

void Human::setFire(bool value)
{
    shotEffect = value;
}

QPointF Human::getTarget() const
{
    return target;
}

void Human::setTarget(const QPointF &value)
{
    target = value;
}



