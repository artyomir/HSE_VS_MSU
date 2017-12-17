#include "target.h"

static int randomBetween(int low, int high)
{
    return (qrand() % ((high + 1) - low) + low);
}

Target::Target(QObject *parent) :
    QObject(parent), QGraphicsItem()
{
    health = randomBetween(1,15);
    maxHealth = health;
    shot = true;
    bulletTimer = new QTimer();
    connect(bulletTimer, &QTimer::timeout, this, &Target::slotBulletTimer);
    bulletTimer->start(1000/1);
    targetHSE = QPointF(qrand(), qrand());
}

Target::~Target()
{
    qDebug() << "target destructor was called";
    delete bulletTimer;
}

QRectF Target::boundingRect() const
{
    return QRectF(-20,-20,40,40);
}

void Target::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setPen(Qt::black);
    painter->setBrush(QColor(255, 0, 0,/* (17 * health) % */254));
    painter->drawRect(-20,-10,40,30);

    painter->setPen(Qt::NoPen);
    painter->setBrush(Qt::red);
    painter->drawRect(-20,-20, (int) 40*health/maxHealth,3);

    Q_UNUSED(option);
    Q_UNUSED(widget);
}

void Target::hit(int damage)
{
    health -= damage;
    this->update(QRectF(-20,-20,40,40));
    if(health <= 0) this->deleteLater();
}

void Target::slotTarget(QPointF point)
{
    Q_UNUSED(point);
}

void Target::slotBulletTimer()
{
    if(shot) emit signalBullet(mapToScene(0,-25), QPointF(randomBetween(0, 600), randomBetween(0, 600)), this);
}

void Target::slotShot(bool shot)
{
    Q_UNUSED(shot)
    this->shot = true;
}

ObjectType Target::getType(){
    return TARGET;
}

void Target::slotGameTimer(){
    return;
}

