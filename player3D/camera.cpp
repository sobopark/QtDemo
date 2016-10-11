#include "camera.h"

#include <QString>
#include <QFileDialog>
#include <QLabel>
#include <QGraphicsScene>
#include <QGraphicsView>

#include <QPainter>
#include <QPixmap>
#include <QImage>

#include <QCamera>
#include <QCameraViewfinder>
#include <QCameraImageCapture>
#include <QFileDialog>

#include <iostream>
#include <sstream>
using namespace std;
Camera::Camera()
{
    ImgCount = 0;
    Gray_Flag = 0;
}

void Camera::save_camera()
{
    stringstream stemp;
    stemp<<ImgCount;
    string scount = stemp.str();

    string str="E:\\work\\Github\\QT\QtDemo\\pic\\left";
    str += scount;
    str += ".jpg";

    stringstream stempr;
    stempr<<ImgCount;
    string scountr = stempr.str();

    string strr="E:\\work\\Github\\QT\QtDemo\\pic\\right";
    strr += scountr;
    strr += ".jpg";


    //QString qstr = QString::fromStdString(str);
    //ui->label_test->setText(qstr);


    imwrite(str,srcImageL);
    imwrite(strr,srcImageR);
    //imwrite("F:\\Code\\QT\\CoCamera\\pic\\left\\ImgR.jpg",srcImageR);
    ImgCount++;

}

void Camera::convert2gray()
{
    Gray_Flag = !Gray_Flag;
}
