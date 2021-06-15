#ifndef Human_H
#define Human_H
#include <QPainter>
#include <QGraphicsItem>
#include <QObject>
#include <QPolygon>
#include <QTimer>
#include <QGraphicsScene>

#include <windows.h>
class Human : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    explicit Human(QObject *parent = 0);
    QRectF boundingRect() const;
    QPointF getTarget() const;
    void setTarget(const QPointF &value);

    bool getFire() const;
    void setFire(bool value);

    bool getAllowshoot() const;
    void setAllowshoot(bool value);

    void shot();
    void shotEnd();

    int getAmmoP() const;
    void setAmmoP(int value);

    int getHp() const;
    void setHp(int value);

    int getMoney() const;
    void setMoney(int value);

    int getAmmoSum() const;
    void setAmmoSum(int value);

    int getStep() const;
    void setStep(int value);

signals:
    void signalPlayerCoordinate(QPointF point);

public slots:
    void slotTarget(QPointF point);
//    void reloadend();

private:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);


private slots:
    void slotGameTimer();   // Игровой слот

private:
    QTimer *gameTimer;      // Игровой таймер
    QPointF target;         // Положение курсора
    int mouseX, mouseY;
    bool shotEffect=0;
    bool allowShoot=1;
    QTimer *fireTimer;
    int hp=100;
    int speed=2;
    int ammoP=7;
    int ammoSum=10;
    int money=0;
    int step=0;
};

#endif // Human_H
