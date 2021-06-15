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
#include <QSound>

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
    scene->addRect(0,0,3000,3000);
    ui->graphicsView->setMouseTracking(true);


    QGraphicsPixmapItem *item = scene->addPixmap(QPixmap("fence.png").scaled(1500,50));
    item->moveBy(0,-70);
    item = scene->addPixmap(QPixmap("fence.png").scaled(1500,50));
    item->moveBy(1500,-70);
    item = scene->addPixmap(QPixmap("fence.png").scaled(1500,50));
    item->setRotation(90);
    item->moveBy(-20,0);
    item = scene->addPixmap(QPixmap("fence.png").scaled(1500,50));
    item->setRotation(90);
    item->moveBy(-20,1500);


    item = scene->addPixmap(QPixmap("fence.png").scaled(1500,50));
    item->setRotation(90);
    item->moveBy(3070,0);
    item = scene->addPixmap(QPixmap("fence.png").scaled(1500,50));
    item->setRotation(90);
    item->moveBy(3070,1500);


    item = scene->addPixmap(QPixmap("fence.png").scaled(1500,50));
    item->moveBy(0,3020);
    item = scene->addPixmap(QPixmap("fence.png").scaled(1500,50));
    item->moveBy(1500,3020);

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
    hp=new QGraphicsTextItem;
    hp->setPlainText(QString("HP%1").arg(h->getHp()));
    hp->setScale(4);
    hp->setDefaultTextColor(QColor("white"));
    hp->setFont(QFont ("Mistral"));
    scene->addItem(hp);

    ammo=new QGraphicsTextItem;
    ammo->setScale(4);
    ammo->setFont(QFont ("Mistral"));
    ammo->setDefaultTextColor(QColor("white"));
    ammo->setPlainText(QString("%1/%2").arg(h->getAmmoP()).arg(h->getAmmoSum()));
    scene->addItem(ammo);



    tm=new QGraphicsTextItem;
    tm->setScale(4);
    tm->setFont(QFont ("Mistral"));
    tm->setDefaultTextColor(QColor("white"));
    tm->setPlainText(QString("%1").arg(time));
    scene->addItem(tm);


}


void Widget::update()
{
//    QElapsedTimer timer;
//    timer.start();
    QRect rect(h->pos().rx(),h->pos().ry(),55,55);
    scene->setSceneRect(rect);
    QPointF k={h->pos().y()-z->pos().y(),h->pos().x()-z->pos().x()};
    QPoint s(0,0);

    for(int i=0; i<zombies.size();i++){
        zombies.at(i)->move(QPoint(h->x(),h->y()));
        if (zombies.at(i)->getTime()>=10){
            zombies.at(i)->deleteLater();
            zombies.removeOne(zombies.at(i));
            break;
        }
        if (zombies.at(i)->getAnimationstage()==4 && (zombies.at(i)->pos().x()-h->pos().x())*(zombies.at(i)->pos().x()-h->pos().x())+(zombies.at(i)->pos().y()-h->pos().y())*(zombies.at(i)->pos().y()-h->pos().y())<=1.2*4*50*50){
            h->setHp(h->getHp()-10);
            zombies.at(i)->setAnimationstage(zombies.at(i)->getAnimationstage()+1);
        }
    }

    hp->setPlainText(QString("HP:%1").arg(h->getHp()));
    hp->setPos(h->pos().rx()-850,h->pos().ry()-500);
    ammo->setPlainText(QString("%1/%2").arg(h->getAmmoP()).arg(h->getAmmoSum()));
    ammo->setPos(h->pos().rx()+800,h->pos().ry()-500);
    tm->setPlainText(QString("%1").arg(time));
    tm->setPos(h->pos().rx(),h->pos().ry()-500);

    if (h->getHp()<=0)
        qApp->exit();

    if (time==200)
        qApp->exit();

//    qDebug() << "The slow operation took" << timer.elapsed() << "milliseconds";
//    QCursor::setPos(QCursor::pos().x()+(qrand() % 2 - 1),QCursor::pos().y()+(qrand() % 2 - 1));

}


void Widget::hit(QGraphicsItem *b)
{
    foreach (QGraphicsItem* targ, scene->items()) {
        if (targ==b){
            Zombie *t = qgraphicsitem_cast <Zombie *> (targ);
            t->hit(qrand()%150);
            if(t->getHp()<=0)
                h->setMoney(h->getMoney()+100);
        }
    }
}

void Widget::levels()
{
    time++;
    static int o=0;
    if (time%10==0 && zombies.size()<20){
        o=0;
        while(o<time/10 && zombies.size()<30){
            z=new Zombie();
            z->setPos(qrand()%3000,qrand()%3000);
            zombies.append(z);
            scene->addItem(zombies[zombies.size()-1]);
        o=o+1;
        }
    }
}




void Widget::mousePressEvent(QMouseEvent *event)
{
    if (h->getAllowshoot()){
        if(h->getAmmoP()>0){
            b=new Bullet(h->pos(), h->getTarget());
            connect(b, &Bullet::signalBullet, this, &Widget::hit);
            scene->addItem(b);
            h->shot();
            h->setAmmoP(h->getAmmoP()-1);
            QCursor::setPos(QCursor::pos().x()+(qrand() % 200 - 100),QCursor::pos().y()+(qrand() % 200 - 100));
            QSound::play("gunshot.wav");
        }
        else
            QSound::play("dry.wav");
    }


}

Widget::~Widget()
{
    delete ui;
}

void Widget::keyPressEvent(QKeyEvent *e)
{
    if(e->key()=='1' && h->getMoney()>=50){
        h->setAmmoP(h->getAmmoP()+1);
        h->setMoney(h->getMoney()-50);
    }
    if(e->key()=='0'){
        qDebug()<<"AMMO"<<(h->getAmmoP());
        qDebug()<<"MONEY"<<(h->getMoney());
        qDebug()<<"HP"<<h->getHp();
    }
    if(e->key()=='R' && h->getAmmoSum()>0){
        while(h->getAmmoP()<7)
        {
            h->setAmmoP(h->getAmmoP()+1);
            h->setAmmoSum(h->getAmmoSum()-1);
        }
        QSound::play("reload.wav");
    }

}




