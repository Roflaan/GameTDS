#ifndef BULLET_H
#define BULLET_H

#include <QObject>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QTimer>
#include <QPainter>
#include <zombie.h>
class Bullet : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    explicit Bullet(QPointF start, QPointF end, QObject *parent = 0);
    ~Bullet();

signals:
    void signalBullet(QGraphicsItem *b);

public slots:

private:

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
    QTimer *timerBullet;    // Слот для обработки таймера пули

private slots:
    void slotTimerBullet(); // Слот для обработки полёта пули
};

#endif // BULLET_H
