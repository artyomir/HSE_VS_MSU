#ifndef SHOOTER_H
#define SHOOTER_H

#include <QObject>
#include <QGraphicsItem>
#include <QPainter>
#include <QPolygon>
#include <QTimer>
#include <QDebug>
#include <QCursor>
#include "target.h"

class Shooter: public Target
{
    Q_OBJECT
public:
    Shooter(QObject *parent = 0);
    ~Shooter();
    void hit(int damage);
    ObjectType getType();
signals:
//    void signalBullet(QPointF start, QPointF end, Target * shooter);
    void signalDie();
public slots:
    void slotTarget(QPointF point);
    void slotShot(bool shot);
private slots:
    void slotGameTimer();
    void slotBulletTimer();
private:
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QPainterPath shape() const;
    QTimer *gameTimer;
    QPointF target;
//    bool shot;
//    QTimer *bulletTimer;
//    int health;
//    int maxHealth;
};

#endif // SHOOTER_H
