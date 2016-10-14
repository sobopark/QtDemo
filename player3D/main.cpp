#include "player3d.h"
#include <QApplication>
#include "vlfeats.h"
#include "opencvsift.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //player3D w;
    opencvsift tmp;
    tmp.siftDtc();
    //w.show();

    return a.exec();
}
