#ifndef CUSTOMESCENE_H
#define CUSTOMESCENE_H

#include <QObject>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>

class CustomeScene: public QGraphicsScene
{
    Q_OBJECT
public:
    explicit CustomeScene(QObject *parent = 0);
    ~CustomeScene();

signals:
    void signalTargetCoordinate(QPointF point);
    void signalShot(bool shot); // Сигнал на стрельбу

public slots:

private:
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
};

#endif // CUSTOMESCENE_H
