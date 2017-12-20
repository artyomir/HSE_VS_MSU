#include "widget.h"
#include <QApplication>

#include "registration.h"
#include "objvec.h"
#include "authorization.h"
int main(int argc, char *argv[])
{
    ObjVec <int> vec;
    vec.append(1);
    vec.append(2);
    vec.append(3);
    vec.append(4);
    vec.append(5);
    vec.append(6);
    vec.append(7);
    vec.append(8);
    vec.remove(7);
    if (vec.isEmpty())
        qDebug() << "net";
    else
        qDebug() << "da";
    size_t n = vec.size();
    for (int i = 0; i < n; ++i){
        qDebug() << vec[i] << "\n";
    }
    QApplication a(argc, argv);
    Authorization w;
    w.show();

    return a.exec();
}

