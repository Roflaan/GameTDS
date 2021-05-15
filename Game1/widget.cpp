#include "widget.h"
#include "ui_widget.h"
#include <windows.h>
#include <winuser.h>
#include <QPainter>
#include <QMouseEvent>
#include <QTimer>
#include <QThread>
#include <QPen>
#include "human.h"
#include <windows.h>
#include <winuser.h>
#include <QDebug>
#include <QGraphicsView>
#include "ui_widget.h"
#include "bullet.h"
#include <QFuture>
#include <QtConcurrent/QtConcurrent>
#pragma comment(lib, "user32.lib")
using namespace std;
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    this->resize(1280,720);
    this->setFixedSize(1280,720);
    this->setFixedSize(1920,1080);

    ui->setupUi(this);
    scene   = new CustomScene();    // Инициализируем кастомизированную сцену

    ui->graphicsView->setScene(scene);  /// Устанавливаем графическую сцену в graphicsView
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff); /// Отключаем скроллбар по вертикали
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff); /// Отключаем скроллбар по горизонтали

    QCursor cursor = QCursor(QPixmap("cursorTarget.png"));
    ui->graphicsView->setCursor(cursor);    // Устанавливаем курсор в QGraphicsView
//    ui->graphicsView->setCursor(Qt::BlankCursor);
    h=new Human();
    h->setPos(650,350);
    scene->addItem(h);
    ui->graphicsView->setMouseTracking(true);


    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(update()));
    connect(timer,SIGNAL(timeout()),h,SLOT(slotGameTimer()));
    timer->start(15);

    connect(scene, &CustomScene::signalTargetCoordinate, h, &Human::slotTarget);

    levelTimer = new QTimer(this);
    connect(levelTimer,SIGNAL(timeout()),this,SLOT(levels()));
    levelTimer->start(1000);


    z=new Zombie();
    z->setPos(0,0);
    zombies.append(z);
    scene->addItem(zombies[0]);



    QBrush b(QPixmap("map1.jpeg").scaled(size()));
    scene->setBackgroundBrush(b);

    b={QPixmap("fog.png").scaled(size())};
    scene->setForegroundBrush(b);



}


void Widget::update()
{
    QElapsedTimer timer;
    timer.start();
    QRect rect(h->pos().rx(),h->pos().ry(),55,55);
    scene->setSceneRect(rect);
    QPointF k={h->pos().y()-z->pos().y(),h->pos().x()-z->pos().x()};
    QPoint s(0,0);

    for(int i=0; i<zombies.size();i++)
        zombies.at(i)->move(QPoint(h->x(),h->y()));
    qDebug() << "The slow operation took" << timer.elapsed() << "milliseconds";
}


void Widget::hit(QGraphicsItem *b)
{
    foreach (QGraphicsItem* targ, scene->items()) {
        if (targ==b){
            Zombie *t = qgraphicsitem_cast <Zombie *> (targ);
            t->hit(50);
        }
    }
}

void Widget::levels()
{
    static int o=0;
    while(o<20){
    z=new Zombie();
    z->setPos(qrand()%3000,qrand()%3000);
    zombies.append(z);
    scene->addItem(zombies[zombies.size()-1]);
    o=o+1;
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


