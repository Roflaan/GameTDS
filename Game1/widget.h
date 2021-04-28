#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <human.h>
#include <QTimer>
#include <customscene.h>
#include <bullet.h>
#include <zombie.h>>

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

private:
    Ui::Widget *ui;
    CustomScene  *scene;    // Объявляем графическую сцену
    Human* h;
    Zombie* z;
    Bullet* b;
    int mouseX, mouseY;
    QTimer* timer;
    QVector<Zombie*>zombies;
    QVector<Bullet*>Bullets;


};

#endif // WIDGET_H
