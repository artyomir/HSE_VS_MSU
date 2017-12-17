#include "shooter.h"
#include <math.h>
#include <QGraphicsScene>

static const double Pi = 3.14159265358979323846264338327950288419717;
static double TwoPi = 2.0 * Pi;

static qreal normalizeAngle(qreal angle)
{
    while (angle < 0)
        angle += TwoPi;
    while (angle > TwoPi)
        angle -= TwoPi;
    return angle;
}

Shooter::Shooter(QObject *parent)
    : Target(parent)
{
    setRotation(0);

    target = QPointF(0,0);
    shot = false;

    gameTimer = new QTimer();
    connect(gameTimer, &QTimer::timeout, this, &Shooter::slotGameTimer);
    gameTimer->start(10);

    target = QPointF(0,0);
    health = 15;
    maxHealth = health;

    this->bulletTimer->start(1000/6);
}

Shooter::~Shooter()
{
    qDebug() << "Shooter destructor was called";
    delete gameTimer;
}

QRectF Shooter::boundingRect() const
{
    return QRectF(-15,-15,35,35);
}

void Shooter::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QPolygon polygon;
    polygon << QPoint(0,-15) << QPoint(10,15) << QPoint(-10,15);
    painter->setBrush(Qt::blue);
    painter->drawPolygon(polygon);

    painter->setPen(Qt::NoPen);
    painter->setBrush(Qt::green);
    painter->drawRect(-10,18, 20  * health/maxHealth ,2);

    Q_UNUSED(option);
    Q_UNUSED(widget);
}

void Shooter::slotTarget(QPointF point)
{
    target = point;
    QLineF lineToTarget(QPointF(0, 0), mapFromScene(target));
    qreal angleToTarget = ::acos(lineToTarget.dx() / lineToTarget.length());
    if (lineToTarget.dy() < 0)
        angleToTarget = TwoPi - angleToTarget;
    angleToTarget = normalizeAngle((Pi - angleToTarget) + Pi / 2);
    if (angleToTarget >= 0 && angleToTarget < Pi) {
        setRotation(rotation() - angleToTarget * 180 /Pi);
    } else if (angleToTarget <= TwoPi && angleToTarget > Pi) {
        setRotation(rotation() + (angleToTarget - TwoPi )* (-180) /Pi);
    }
}

void Shooter::slotGameTimer()
{
    if(GetAsyncKeyState('A')){
        this->setX(this->x() - 1);
        if(!scene()->collidingItems(this).isEmpty()){
            this->setX(this->x() + 1);
        }
    }
    if(GetAsyncKeyState('D')){
        this->setX(this->x() + 1);
        if(!scene()->collidingItems(this).isEmpty()){
            this->setX(this->x() - 1);
        }
    }
    if(GetAsyncKeyState('W')){
        this->setY(this->y() - 1);
        if(!scene()->collidingItems(this).isEmpty()){
            this->setY(this->y() + 1);
        }
    }
    if(GetAsyncKeyState('S')){
        this->setY(this->y() + 1);
        if(!scene()->collidingItems(this).isEmpty()){
            this->setY(this->y() - 1);
        }
    }
    if(GetAsyncKeyState(VK_LEFT)){
        this->setX(this->x() - 1);
        if(!scene()->collidingItems(this).isEmpty()){
            this->setX(this->x() + 1);
        }
    }
    if(GetAsyncKeyState(VK_RIGHT)){
        this->setX(this->x() + 1);
        if(!scene()->collidingItems(this).isEmpty()){
            this->setX(this->x() - 1);
        }
    }
    if(GetAsyncKeyState(VK_UP)){
        this->setY(this->y() - 1);
        if(!scene()->collidingItems(this).isEmpty()){
            this->setY(this->y() + 1);
        }
    }
    if(GetAsyncKeyState(VK_DOWN)){
        this->setY(this->y() + 1);
        if(!scene()->collidingItems(this).isEmpty()){
            this->setY(this->y() - 1);
        }
    }
    if(this->x() - 30 < 0){
        this->setX(30);
    }
    if(this->x() + 30 > 500){
        this->setX(500 - 30);
    }

    if(this->y() - 30 < 0){
        this->setY(30);
    }
    if(this->y() + 30 > 500){
        this->setY(500 - 30);
    }

    QLineF lineToTarget(QPointF(0, 0), mapFromScene(target));
    qreal angleToTarget = ::acos(lineToTarget.dx() / lineToTarget.length());
    if (lineToTarget.dy() < 0)
        angleToTarget = TwoPi - angleToTarget;
    angleToTarget = normalizeAngle((Pi - angleToTarget) + Pi / 2);
    if (angleToTarget >= 0 && angleToTarget < Pi) {
        setRotation(rotation() - angleToTarget * 180 /Pi);
    } else if (angleToTarget <= TwoPi && angleToTarget > Pi) {
        setRotation(rotation() + (angleToTarget - TwoPi )* (-180) /Pi);
    }
}

void Shooter::slotBulletTimer()
{
    if(shot) emit signalBullet(mapToScene(0,-25), target, this);
}

void Shooter::slotShot(bool shot)
{
    this->shot = shot;
}

QPainterPath Shooter::shape() const
{
    QPainterPath path;
    path.addEllipse(boundingRect());
    return path;
}

void Shooter::hit(int damage)
{
    health -= damage;
    this->update(QRectF(-15,-15,35,35));
    if(health <= 0) this->deleteLater();
}

ObjectType Shooter::getType(){
    return SHOOTER;
}

