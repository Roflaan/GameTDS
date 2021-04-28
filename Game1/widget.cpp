#include "widget.h"
#include "ui_widget.h"
#include <windows.h>
#include <winuser.h>
#include <QPainter>
#include <QMouseEvent>
#include <QTimer>
#include <QPen>
#include "human.h"
#include <windows.h>
#include <winuser.h>
#include <QDebug>
#include <QGraphicsView>
#include "ui_widget.h"
#include "bullet.h"
#pragma comment(lib, "user32.lib")
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    // Устанавливаем параметры окна приложения
    this->resize(1280,720);
    this->setFixedSize(1280,720);
    this->setFixedSize(1920,1080);

    ui->setupUi(this);
    scene   = new CustomScene();    // Инициализируем кастомизированную сцену

    ui->graphicsView->setScene(scene);  /// Устанавливаем графическую сцену в graphicsView
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);    /// Устанавливаем сглаживание
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff); /// Отключаем скроллбар по вертикали
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff); /// Отключаем скроллбар по горизонтали

//    scene->addEllipse(100,100, 100, 100);
//    scene->addLine(0,0,6000,6000);
    // Создаем кастомизированный курсор из ресурсного файла
    QCursor cursor = QCursor(QPixmap("cursorTarget.png"));
    ui->graphicsView->setCursor(cursor);    // Устанавливаем курсор в QGraphicsView
    h=new Human();
    h->setPos(650,350);
    scene->addItem(h);
    ui->graphicsView->setMouseTracking(true);


    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(update()));
    connect(timer,SIGNAL(timeout()),h,SLOT(slotGameTimer()));
    timer->start(10);
    connect(scene, &CustomScene::signalTargetCoordinate, h, &Human::slotTarget);

    z=new Zombie();
    z->setPos(0,0);
    zombies.append(z);
    scene->addItem(zombies[0]);


    z=new Zombie();
    z->setPos(500,500);
    zombies.append(z);
    scene->addItem(zombies[1]);


    z=new Zombie();
    z->setPos(250,0);
    zombies.append(z);
    scene->addItem(zombies[2]);

    connect(h, &Human::signalPlayerCoordinate, zombies[0], &Zombie::move);
    connect(h, &Human::signalPlayerCoordinate, zombies[1], &Zombie::move);
    connect(h, &Human::signalPlayerCoordinate, zombies[2], &Zombie::move);



    scene->addLine(-1500,-1500,1500,-1500);
    scene->addLine(-1500,-1500,-1500,1500);
    scene->addLine(-1500,1500,1500,1500);
    scene->addLine(1500,-1500,1500,1500);

    QBrush b(QPixmap("map1.jpeg").scaled(size()));
    scene->setBackgroundBrush(b);

    b={QPixmap("fog.png").scaled(size())};
    scene->setForegroundBrush(b);

}


void Widget::update()
{
    QRect rect(h->pos().rx(),h->pos().ry(),55,55);
    scene->setSceneRect(rect);
    QPointF k={h->pos().y()-z->pos().y(),h->pos().x()-z->pos().x()};
}

void Widget::hit(QGraphicsItem *b)
{
    foreach (QGraphicsItem* targ, scene->items()) {
        if (targ==b){
            Zombie *t = qgraphicsitem_cast <Zombie *> (targ);
            t->hit(50);
        }
//        repaint();

    }
}



void Widget::mousePressEvent(QMouseEvent *event)
{
    if (h->getAllowshoot()){
        b=new Bullet(h->pos(), h->getTarget());
        connect(b, &Bullet::signalBullet, this, &Widget::hit);
        scene->addItem(b);
        h->shot();
    }
}

Widget::~Widget()
{
    delete ui;
}


