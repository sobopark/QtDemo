#include "player3d.h"
#include <QApplication>
#include "vlfeats.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    player3D w;
    vlfeats tmp;
    int ss=tmp.getSift();
    w.show();

    return a.exec();
}
