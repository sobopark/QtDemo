#include "player3d.h"
#include <QApplication>
#include "opencvsift.h"
//bool comparison(DMatch a,DMatch b)
//{
//    return a.distance<b.distance;
//}
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    player3D w;
    //opencvsift tmp;

    //Mat img_l=imread("C:\\Users\\ZhongZY\\Desktop\\test_qt\\left.jpg");
    //Mat img_r=imread("C:\\Users\\ZhongZY\\Desktop\\test_qt\\right.jpg");

    w.show();

    return a.exec();
}
