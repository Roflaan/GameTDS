#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <human.h>
#include <QTimer>
#include <customscene.h>
#include <bullet.h>
#include <zombie.h>
#include <QThread>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();



public slots:
    void mousePressEvent(QMouseEvent *event);
    void update();
    void hit(QGraphicsItem *b);
    void levels();

private:
    Ui::Widget *ui;
    CustomScene  *scene;    // Объявляем графическую сцену
    Human* h;
    Zombie* z;
    Bullet* b;
    int mouseX, mouseY;
    QTimer* timer;
    QTimer* levelTimer;
    QList<Zombie*>zombies;
    QVector<Bullet*>Bullets;
};

#endif // WIDGET_H
