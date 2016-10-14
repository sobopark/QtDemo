#include "player3d.h"
#include <QApplication>
#include "opencvsift.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //player3D w;
    opencvsift tmp;
    vector<KeyPoint> re;
    Mat img=imread("C:\\Users\\ZhongZY\\Desktop\\000182.bmp");
    re=tmp.siftDtc(img);
    tmp.siftMat(img,re);
    //w.show();

    return a.exec();
}
