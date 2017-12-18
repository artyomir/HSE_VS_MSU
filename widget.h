#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsItem>

#include "target.h"
#include "bullet.h"
#include "customescene.h"
#include "shooter.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private:
    Ui::Widget *ui;
    CustomeScene  *scene;
    Shooter * shooter;
    QTimer *timerTarget;
    QList<QGraphicsItem *> targets;

public slots:
    void slotHitTarget(QGraphicsItem *item);

private slots:
    void slotCreateTarget();
    void slotBullet(QPointF start, QPointF end, Target *shooter);
};

#endif // WIDGET_H
