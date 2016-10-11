#include "player3d.h"
#include "ui_player3d.h"
#include "camera.h"

// test github push

using namespace cv;
using namespace std;

player3D::player3D(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::player3D)
{
    ui->setupUi(this);

    ui->save_camera->setEnabled(0);
    ui->close_camera->setEnabled(0);


    this->cam = new Camera();

    connect(&theTimer, &QTimer::timeout, this, &player3D::updateImage);
    connect(ui->open_camera, SIGNAL(clicked(bool)), this, SLOT(open_camera()));
    connect(ui->close_camera, SIGNAL(clicked(bool)), this, SLOT(close_camera()));
    connect(ui->save_camera, SIGNAL(clicked(bool)), this->cam, SLOT(save_camera()));
    connect(ui->gray_camera, SIGNAL(clicked(bool)), this->cam, SLOT(convert2gray()));
}

player3D::~player3D()
{
    delete ui;
}

void player3D::updateImage()
{
    if(cam->Gray_Flag == 0)
    {
        cam->videoCapL>>cam->srcImageL;
        if(cam->srcImageL.data)
        {
            cvtColor(cam->srcImageL, cam->srcImageL, CV_BGR2RGB);

            cam->videoCapR>>cam->srcImageR;
            if(cam->srcImageR.data)
            {
                cvtColor(cam->srcImageR, cam->srcImageR, CV_BGR2RGB);
                this->update();
            }
        }
    }
    else
    {
        cam->videoCapL>>cam->srcImageL;
        if(cam->srcImageL.data)
        {
            cvtColor(cam->srcImageL, cam->srcImageL, CV_BGR2GRAY);

            cam->videoCapR>>cam->srcImageR;
            if(cam->srcImageR.data)
            {
                cvtColor(cam->srcImageR, cam->srcImageR, CV_BGR2GRAY);
                this->update();
            }
        }
    }

}

void player3D::paintEvent(QPaintEvent *e)
{

    if(cam->Gray_Flag == 0)
    {

        QImage imagel = QImage((uchar*)(cam->srcImageL.data), cam->srcImageL.cols, cam->srcImageL.rows, QImage::Format_RGB888);
        ui->img_label->setPixmap(QPixmap::fromImage(imagel));
        ui->img_label->resize(imagel.size());
        ui->img_label->show();

        QImage imager = QImage((uchar*)(cam->srcImageR.data), cam->srcImageR.cols, cam->srcImageR.rows, QImage::Format_RGB888);
        ui->img_label2->setPixmap(QPixmap::fromImage(imager));
        ui->img_label2->resize(imager.size());
        ui->img_label2->show();
    }
    else
    {
        QImage imagel = QImage((uchar*)(cam->srcImageL.data), cam->srcImageL.cols, cam->srcImageL.rows,cam->srcImageL.step, QImage::Format_Indexed8);
        ui->img_label->setPixmap(QPixmap::fromImage(imagel));
        ui->img_label->resize(imagel.size());
        ui->img_label->show();

        QImage imager = QImage((uchar*)(cam->srcImageR.data), cam->srcImageR.cols, cam->srcImageR.rows,cam->srcImageR.step, QImage::Format_Indexed8);
        ui->img_label2->setPixmap(QPixmap::fromImage(imager));
        ui->img_label2->resize(imager.size());
        ui->img_label2->show();
    }

}

void player3D::open_camera()
{
    ui->save_camera->setEnabled(1);
    ui->close_camera->setEnabled(1);
    if(cam->videoCapL.open(0))
    {
        cam->srcImageL = Mat::zeros(cam->videoCapL.get(CV_CAP_PROP_FRAME_HEIGHT), cam->videoCapL.get(CV_CAP_PROP_FRAME_WIDTH), CV_8UC3);
        if(cam->videoCapR.open(1))
        {
            cam->srcImageR = Mat::zeros(cam->videoCapR.get(CV_CAP_PROP_FRAME_HEIGHT), cam->videoCapR.get(CV_CAP_PROP_FRAME_WIDTH), CV_8UC3);
            theTimer.start(33);
        }
    }
}

void player3D::close_camera()
{

    theTimer.stop();
    ui->close_camera->setEnabled(0);
}
