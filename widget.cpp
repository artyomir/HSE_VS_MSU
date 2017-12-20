#include "widget.h"
#include "ui_widget.h"
#include "QMessageBox"

#include <QFile>
#include <QDebug>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

static int randomBetween(int low, int high)
{
    return (qrand() % ((high + 1) - low) + low);
}

Widget::Widget(QString user, int record, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget),
    user_(user),
    record(record)
{
//    this->resize(600,600);
    this->setFixedSize(600,600);

    ui->setupUi(this);
    ui->user->setText(this->user_);
    ui->record->setPalette(Qt::red);
    ui->scoresNum->setPalette(Qt::black);
    ui->record->display(record);
    ui->stop->setDisabled(true);

    scene = new CustomeScene();

    ui->graphicsView->setScene(scene);
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    scene->setSceneRect(0,0,500,500);

    QCursor cursor;
    ui->graphicsView->setCursor(cursor);

    scene->addRect(-20,0,520,20,QPen(Qt::NoPen),QBrush(Qt::darkGray));
    scene->addRect(-20,0,20,500,QPen(Qt::NoPen),QBrush(Qt::darkGray));
    scene->addRect(0,480,500,20,QPen(Qt::NoPen),QBrush(Qt::darkGray));
    scene->addRect(500,0,20,500,QPen(Qt::NoPen),QBrush(Qt::darkGray));
}

Widget::~Widget()
{
    delete timerTarget;
    delete scene;
    delete ui;
}

void Widget::slotBullet(QPointF start, QPointF end, Target * shooter)
{
    Bullet* bullet = new Bullet(start, end, shooter);
    connect(bullet, &Bullet::shotHit, this, &Widget::slotHitTarget);
    scene->addItem(bullet);
}

void Widget::slotCreateTarget()
{
    Target *target = new Target();
    connect(scene, &CustomeScene::signalShot, target, &Target::slotShot);
    connect(target, &Target::signalBullet, this, &Widget::slotBullet);
    connect(target, &Target::signalScore, this, &Widget::slotScore);
    connect(this, &Widget::stopSignal, target, &Target::stopSlot);
    scene->addItem(target);
    target->setPos(qrand() % ((500 - 40 + 1) - 40) + 40,
                  qrand() % ((500 - 40 + 1) - 40) + 40);
    target->setZValue(-1);
    targets.append(target);
}

void Widget::slotHitTarget(QGraphicsItem *item)
{
    for (int i = 0; i < targets.size(); ++i){
        if (targets[i] && targets[i] == item){
            Target *t = qgraphicsitem_cast <Target *> (targets[i]);
            if (t)
                if (t->getType() == SHOOTER)
                    t->hit(randomBetween(1,3));
                else
                    t->hit(1);
        }
    }
}

void Widget::slotScore(){
    scores_++;
    ui->scoresNum->display((double)scores_);
    if (scores_ > record){
        record = scores_;
        ui->record->display(record);
    }
}

void Widget::slotDie(){
    if (this->shooter)
        delete shooter;
    timerTarget->stop();
    emit stopSignal();

    QFile mFile ("./pasw_log.txt");
    if(!mFile.open(QFile::ReadOnly | QFile::Text)){
        qDebug() << "File for loading not found\n";
        return;
    }
    QString val = mFile.readAll();
    mFile.close();
    QJsonDocument doc = QJsonDocument::fromJson(val.toUtf8());
    QJsonArray JSArray(doc.array());

    int i = 0;
    foreach (auto now, JSArray) {
        if (now.toObject()["Login"].toString() == this->user_){
            QJsonObject obj;

            obj.insert("Login", QJsonValue::fromVariant(now.toObject()["Login"].toString()));
            obj.insert("Password", QJsonValue::fromVariant(now.toObject()["Password"].toString()));
            obj.insert("Name", QJsonValue::fromVariant(now.toObject()["Name"].toString()));
            obj.insert("Scores", QJsonValue::fromVariant(QString::number(this->record)));

            JSArray.append(obj);
            break;
        }
        ++i;
    }
    JSArray.removeAt(i);
    QJsonDocument doc_(JSArray);
    QFile mFile_("./pasw_log.txt");
    if(!mFile_.open(QFile::WriteOnly)){
    }
    mFile_.write(doc_.toJson());
    mFile_.close();
    ui->stop->setDisabled(true);
    ui->start->setDisabled(false);
    QMessageBox * msgBox = new QMessageBox();
    msgBox->setText("Game over.\n Your scores : " + QString::number(scores_));
    msgBox->setButtonText(1, "OK");
    msgBox->show();
    msgBox->exec();
}


void Widget::on_stop_clicked()
{
    if (this->shooter)
        delete shooter;
    timerTarget->stop();
    emit stopSignal();

    QFile mFile ("./pasw_log.txt");
    if(!mFile.open(QFile::ReadOnly | QFile::Text)){
        qDebug() << "File for loading not found\n";
        return;
    }
    QString val = mFile.readAll();
    mFile.close();
    QJsonDocument doc = QJsonDocument::fromJson(val.toUtf8());
    QJsonArray JSArray(doc.array());

    int i = 0;
    foreach (auto now, JSArray) {
        if (now.toObject()["Login"].toString() == this->user_){
            QJsonObject obj;

            obj.insert("Login", QJsonValue::fromVariant(now.toObject()["Login"].toString()));
            obj.insert("Password", QJsonValue::fromVariant(now.toObject()["Password"].toString()));
            obj.insert("Name", QJsonValue::fromVariant(now.toObject()["Name"].toString()));
            obj.insert("Scores", QJsonValue::fromVariant(QString::number(this->record)));

            JSArray.append(obj);
            break;
        }
        ++i;
    }
    JSArray.removeAt(i);
    QJsonDocument doc_(JSArray);
    QFile mFile_("./pasw_log.txt");
    if(!mFile_.open(QFile::WriteOnly)){
    }
    mFile_.write(doc_.toJson());
    mFile_.close();
    ui->stop->setDisabled(true);
    ui->start->setDisabled(false);
}

void Widget::on_start_clicked()
{
    ui->stop->setDisabled(false);
    ui->start->setDisabled(true);
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
    connect(shooter, &Shooter::signalDie, this, &Widget::slotDie);

    targets.append(shooter);

    scores_ = 0;
}
