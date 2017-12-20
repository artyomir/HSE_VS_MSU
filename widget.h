#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsItem>

#include "target.h"
#include "bullet.h"
#include "customescene.h"
#include "shooter.h"
#include "objvec.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QString user = "User", int record = 0 , QWidget *parent = 0);
    ~Widget();

private:
    Ui::Widget *ui;
    CustomeScene  *scene;
    Shooter * shooter;
    QTimer *timerTarget;
//    QList<QGraphicsItem *> targets;
    ObjVec<QGraphicsItem *> targets;
    int scores_;
    QString user_;
    int record;

signals:
    void stopSignal();

public slots:
    void slotHitTarget(QGraphicsItem *item);
    void slotScore();
    void slotDie();

private slots:
    void slotCreateTarget();
    void slotBullet(QPointF start, QPointF end, Target *shooter);
    void on_stop_clicked();
    void on_start_clicked();
};

#endif // WIDGET_H
