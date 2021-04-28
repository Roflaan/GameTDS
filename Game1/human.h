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

signals:
    void signalPlayerCoordinate(QPointF point);

public slots:
    void slotTarget(QPointF point);

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
};

#endif // Human_H
