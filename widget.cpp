#include "widget.h"
#include "ui_widget.h"

static int randomBetween(int low, int high)
{
    return (qrand() % ((high + 1) - low) + low);
}

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    this->resize(600,600);
    this->setFixedSize(600,600);

    ui->setupUi(this);
    scene = new CustomeScene();

    ui->graphicsView->setScene(scene);
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);    /// Устанавливаем сглаживание
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff); /// Отключаем скроллбар по вертикали
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff); /// Отключаем скроллбар по горизонтали

    scene->setSceneRect(0,0,500,500);

    QCursor cursor;
    ui->graphicsView->setCursor(cursor);

    shooter = new Shooter();
    shooter->setPos(200,200);
    scene->addItem(shooter);

    ui->graphicsView->setMouseTracking(true);

    timerTarget = new QTimer();
    connect(timerTarget, &QTimer::timeout, this, &Widget::slotCreateTarget);
    timerTarget->start(1500);

    connect(scene, &CustomeScene::signalTargetCoordinate, shooter, &Shooter::slotTarget);
    connect(scene, &CustomeScene::signalShot, shooter, &Shooter::slotShot);
    connect(shooter, &Shooter::signalBullet, this, &Widget::slotBullet);

    scene->addRect(0,0,520,20,QPen(Qt::NoPen),QBrush(Qt::darkGray));
    scene->addRect(0,0,20,520,QPen(Qt::NoPen),QBrush(Qt::darkGray));
    scene->addRect(0,500,520,20,QPen(Qt::NoPen),QBrush(Qt::darkGray));
    scene->addRect(500,0,20,520,QPen(Qt::NoPen),QBrush(Qt::darkGray));
//    scene->addRect(170,250,180,20,QPen(Qt::NoPen),QBrush(Qt::darkGray));
    scene->addRect(250,170,20,180,QPen(Qt::NoPen),QBrush(Qt::darkGray));

    targets.append(shooter);
}

Widget::~Widget()
{
    delete shooter;
    delete timerTarget;
    delete ui;
}

void Widget::slotBullet(QPointF start, QPointF end, Target * shooter)
{
    Bullet* bullet = new Bullet(start, end, shooter);
    connect(bullet, &Bullet::shotHit, this, &Widget::slotHitTarget);
//    bullet->setCallbackFunc(slotHitTarget);
    scene->addItem(bullet);
}

void Widget::slotCreateTarget()
{
    Target *target = new Target();
    connect(scene, &CustomeScene::signalShot, target, &Target::slotShot);
    connect(target, &Target::signalBullet, this, &Widget::slotBullet);
    scene->addItem(target);
    target->setPos(qrand() % ((500 - 40 + 1) - 40) + 40,
                  qrand() % ((500 - 40 + 1) - 40) + 40);
    target->setZValue(-1);
    targets.append(target);
}

void Widget::slotHitTarget(QGraphicsItem *item)
{
    foreach (QGraphicsItem *targ, targets) {
        if(targ == item){
            Target *t = qgraphicsitem_cast <Target *> (targ);
            if (t)
                t->hit(randomBetween(1,3));
        }
    }
}

//QList<QGraphicsItem *> Widget::targets;
