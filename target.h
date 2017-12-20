#ifndef TARGET_H
#define TARGET_H

#include <QObject>
#include <QGraphicsItem>
#include <QPainter>
#include <QTimer>
#include <windows.h>
#include "bullet.h"

typedef enum{SHOOTER, TARGET} ObjectType;

class Target: /*public QObject,*/ public QGraphicsObject
{
    Q_OBJECT
public:
    explicit Target(QObject *parent = 0);
    ~Target();
    virtual void hit(int damage);
    virtual ObjectType getType();

    int health;
    int maxHealth;
    bool shot;
    QTimer *bulletTimer;

signals:
    void signalBullet(QPointF start, QPointF end, Target * shooter);
    void signalScore();

public slots:
    virtual void slotGameTimer();
    virtual void slotTarget(QPointF point);
    virtual void slotShot(bool shot);
    void stopSlot();

private slots:
    virtual void slotBulletTimer();
protected:
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
    QPointF targetHSE;
};

#endif // TARGET_H
