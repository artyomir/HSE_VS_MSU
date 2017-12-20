#ifndef AUTHORIZATION_H
#define AUTHORIZATION_H

#include <QWidget>

namespace Ui {
class Authorization;
}

class Authorization : public QWidget
{
    Q_OBJECT

public:
    explicit Authorization(QWidget *parent = 0);
    ~Authorization();
    bool CheckValid();

private slots:
    void VhodEnabled();

    void on_buttonEnt_clicked();

    void on_buttonReg_clicked();

private:
    Ui::Authorization *ui;
};

#endif // AUTHORIZATION_H
