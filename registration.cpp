#include "registration.h"
#include "ui_registration.h"

#include <QFile>
#include <QDebug>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

Registration::Registration(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Registration)
{
    ui->setupUi(this);
    setWindowTitle("Registration");
    setWindowIcon(QIcon(":/fill/ICON.png"));

    QRegExp ex("[a-zA-Z]{3,15}");
    QRegExp exp("[a-zA-Z]{3,15}[0-9]{0,15}[.,_]{0,5}");
    ui->login->setValidator(new QRegExpValidator(exp, this));
    ui->passw->setValidator(new QRegExpValidator(exp, this));
    ui->name->setValidator(new QRegExpValidator(ex, this));
    ui->secname->setValidator(new QRegExpValidator(ex, this));
    ui->passw2->setValidator(new QRegExpValidator(exp, this));
    connect(ui->name, SIGNAL(textChanged(QString)), this, SLOT(RegEnabled()));
    connect(ui->secname, SIGNAL(textChanged(QString)), this, SLOT(RegEnabled()));
    connect(ui->login, SIGNAL(textChanged(QString)), this, SLOT(RegEnabled()));
    connect(ui->passw, SIGNAL(textChanged(QString)), this, SLOT(RegEnabled()));
    connect(ui->passw2, SIGNAL(textChanged(QString)), this, SLOT(RegEnabled()));
}


Registration::~Registration()
{
    delete ui;
}

bool Registration::CheckValid(){
    if (ui->login->hasAcceptableInput() && ui->passw->hasAcceptableInput() && ui->passw2->hasAcceptableInput()){
        if (ui->secname->hasAcceptableInput() && ui->name->hasAcceptableInput()){
            return true;
        } else return false;
    } else return false;
}

bool Registration::CheckLog(){
    if (ui->passw->text().isEmpty() || ui->passw2->text().isEmpty()
            || ui->login->text().isEmpty() || ui->name->text().isEmpty()
            || ui->secname->text().isEmpty()){
        ui->ErrorInf->setText("<font color=red>Fill in all the fields.</font>");
        return false;
    }
    if (ui->passw2->text() == ui->passw->text()){
        QFile f("./pasw_log.txt");
        if (!f.exists()){
            f.open(QFile::WriteOnly);
        }
        f.close();
        QFile mFile ("./pasw_log.txt");
        if(!mFile.open(QFile::ReadOnly | QFile::Text)){
            //ui->ErrorInf->setText("<font color=red>Ошибка регистрации.</font>");
            qDebug() << "File for loading not found\n";
            return false;
        }
        QString val = mFile.readAll();
        mFile.close();
        QJsonDocument doc = QJsonDocument::fromJson(val.toUtf8());
        QJsonArray JSArray(doc.array());
        foreach (auto now, JSArray) {
            if (now.toObject()["Login"].toString() == ui->login->text()){
                ui->ErrorInf->setText("<font color=red>Username not available</font>");
                return false;
            }
        }
        ui->ErrorInf->setText("<font color=green>Registration complete sucsessfuly</font>");
        return true;
    } else{
        ui->ErrorInf->setText("<font color=red>Error entering password.</font>");
        return false;
    }
    return false;
}


void Registration::on_pushButton_clicked(){
    if (CheckLog()){
        QFile mFile ("./pasw_log.txt");
        if(!mFile.open(QFile::ReadOnly | QFile::Text)){
            qDebug() << "File for loading not found\n";
            return;
        }
        QString val = mFile.readAll();
        mFile.close();
        QJsonDocument doc = QJsonDocument::fromJson(val.toUtf8());
        QJsonArray JSArray(doc.array());
        QFile mFile_("./pasw_log.txt");
        if(!mFile_.open(QFile::WriteOnly)){
            ui->ErrorInf->setText("<font color=red>Registration error.</font>");
            return;
        }
        QJsonObject obj;
        obj.insert("Login", QJsonValue::fromVariant(ui->login->text()));
        obj.insert("Password", QJsonValue::fromVariant(ui->passw->text()));
        obj.insert("Name", QJsonValue::fromVariant(ui->name->text() + " " + ui->secname->text()));
        obj.insert("Scores", QJsonValue::fromVariant("0"));
        JSArray.push_back(obj);
        QJsonDocument doc_(JSArray);
        ui->pushButton->setEnabled(false);
        mFile_.write(doc_.toJson());
        mFile_.close();
        this->close();
    }
}
