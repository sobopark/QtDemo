#include "player3d.h"
#include "ui_player3d.h"
#include "camera.h"

#include <QtWidgets>
#include <QKeyEvent>

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

//    setWindowState(Qt::WindowMaximized);
    setWindowFlags(Qt::FramelessWindowHint);
    showFullScreen();
    setFixedSize(this->width(),this->height());
    statusBar()->hide();

    swapFlag = false;
    crop_left = 0;
    crop_right = 0;

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
        QImage imagel;
        QImage imager;

        if(swapFlag){
            imagel = QImage((uchar*)(cam->srcImageL.data), cam->srcImageL.cols, cam->srcImageL.rows, QImage::Format_RGB888);
            imager = QImage((uchar*)(cam->srcImageR.data), cam->srcImageR.cols, cam->srcImageR.rows, QImage::Format_RGB888);
        }else{
            imager = QImage((uchar*)(cam->srcImageL.data), cam->srcImageL.cols, cam->srcImageL.rows, QImage::Format_RGB888);
            imagel = QImage((uchar*)(cam->srcImageR.data), cam->srcImageR.cols, cam->srcImageR.rows, QImage::Format_RGB888);
        }

        QImage imagel_resize = imagel.scaled(QSize(ui->groupBox->width(),ui->groupBox->height()));
        QImage imager_resize = imager.scaled(QSize(ui->groupBox_2->width(),ui->groupBox_2->height()));

        ui->img_label->setPixmap(QPixmap::fromImage(imagel_resize));
        ui->img_label->resize(imagel_resize.size());
        ui->img_label->show();

        QSize image1_size = imagel.size();
        QSize imager_size = imager.size();
        int result_size_width = image1_size.width() + imager_size.width();
        int resutl_size_height = image1_size.height();

        imagel = imagel.copy(QRect(crop_left,0,image1_size.width() - crop_left,image1_size.height()));
        imagel = imagel.scaled(QSize(image1_size.width(),image1_size.height()));
        imager = imager.copy(QRect(0,0,imager_size.width() - crop_right,imager_size.height()));
        imager = imager.scaled(QSize(imager_size.width(),imager_size.height()));

        QImage image_result = QImage(result_size_width,resutl_size_height,QImage::Format_RGB888);
        QPainter painter(&image_result);
        painter.drawImage(0,0,imagel);
        painter.drawImage(image1_size.width(),0,imager);
        painter.end();
        QImage image_result_resize = image_result.scaled(QSize(ui->groupBox_2->width(),ui->groupBox_2->height()));

        ui->img_label2->setPixmap(QPixmap::fromImage(image_result_resize));
        ui->img_label2->resize(image_result_resize.size());
//        ui->img_label->move(ui->groupBox_2->pos());
        ui->img_label2->show();
    }
    else
    {
        QImage imagel = QImage((uchar*)(cam->srcImageL.data), cam->srcImageL.cols, cam->srcImageL.rows,cam->srcImageL.step, QImage::Format_Indexed8);
        ui->img_label->setPixmap(QPixmap::fromImage(imagel));
        ui->img_label->resize(imagel.size());
//        ui->img_label->move(ui->groupBox->pos());
        ui->img_label->show();

        QImage imager = QImage((uchar*)(cam->srcImageR.data), cam->srcImageR.cols, cam->srcImageR.rows,cam->srcImageR.step, QImage::Format_Indexed8);
        ui->img_label2->setPixmap(QPixmap::fromImage(imager));
        ui->img_label2->resize(imager.size());
//        ui->img_label->move(ui->groupBox_2->pos());
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

void player3D::contextMenuEvent(QContextMenuEvent *e)
{
    QCursor cursor = this->cursor();
    QMenu *menu = new QMenu(this);
    QAction *deleteAction = new QAction(tr("close"),this);
    menu->addAction(deleteAction);
    connect(deleteAction,SIGNAL(triggered()),SLOT(close()));
    menu->exec(cursor.pos());
}

void player3D::keyPressEvent(QKeyEvent *e)
{
    switch(e->key()){
    case Qt::Key_W:
        break;
    case Qt::Key_S:
        break;
    case Qt::Key_A:
        crop_left++;
        break;
    case Qt::Key_D:
        crop_right++;
        break;
    case Qt::Key_Asterisk:
        swapFlag = !swapFlag;
        break;
    case Qt::Key_BackForward:
        crop_left = 0;
        crop_right = 0;
        break;
    }
}
