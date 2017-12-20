#include "authorization.h"
#include "ui_authorization.h"
#include "registration.h"
#include "widget.h"
#include <QFile>
#include <QDebug>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

Authorization::Authorization(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Authorization)
{
    ui->setupUi(this);
    QRegExp exp("[a-zA-Z]{3,15}[0-9]{0,15}[.,_]{0,5}");
    ui->login->setValidator(new QRegExpValidator(exp, this));
    ui->password->setValidator(new QRegExpValidator(exp, this));
    connect(ui->login, SIGNAL(textChanged(QString)),this, SLOT(VhodEnabled()));
    connect(ui->password, SIGNAL(textChanged(QString)),this, SLOT(VhodEnabled()));
}

Authorization::~Authorization()
{
    delete ui;
}

bool Authorization::CheckValid(){
    if (ui->login->hasAcceptableInput() && ui->password->hasAcceptableInput()){
        return true;
    } else return false;
}

void Authorization::VhodEnabled(){
    ui->buttonEnt->setEnabled(CheckValid());
}

void Authorization::on_buttonEnt_clicked()
{
    QFile f("./pasw_log.txt");
    if (!f.exists()){
        f.open(QFile::WriteOnly);
    }
    f.close();
    QFile mFile ("./pasw_log.txt");
    if(!mFile.open(QFile::ReadOnly | QFile::Text)){
        ui->info->setText("<font color=red>Ошибка регистрации.</font>");
        return;
    }
    QString val = mFile.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(val.toUtf8());
    QJsonArray JSArray(doc.array());
    mFile.close();
    foreach (auto now, JSArray) {
        if (now.toObject()["Login"].toString() == ui->login->text()
            && now.toObject()["Password"].toString() == ui->password->text()){
            int scores = now.toObject()["Scores"].toString().toInt();
            Widget * w = new Widget(now.toObject()["Login"].toString(), scores, 0);
            w->show();
            this->close();
        }
    }
    ui->info->setText("<font color=red>Incorrect login or password.</font>");
}

void Authorization::on_buttonReg_clicked()
{
    Registration w;
    w.show();
    w.exec();
}
