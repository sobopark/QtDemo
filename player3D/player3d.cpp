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

    this->cam = new Camera();

//    setWindowState(Qt::WindowMaximized);
    setWindowFlags(Qt::FramelessWindowHint);
    showFullScreen();
    setFixedSize(this->width(),this->height());
    statusBar()->hide();

    swapFlag = false;

    image_left_crop_up = 0;
    image_left_crop_down = 0;
    image_left_crop_left = 0;
    image_left_crop_right = 0;
    image_right_crop_up = 0;
    image_right_crop_down = 0;
    image_right_crop_left = 0;
    image_right_crop_right = 0;


    connect(&theTimer, &QTimer::timeout, this, &player3D::updateImage);
//    connect(ui->open_camera, SIGNAL(clicked(bool)), this, SLOT(open_camera()));
//    connect(ui->close_camera, SIGNAL(clicked(bool)), this, SLOT(close_camera()));
//    connect(ui->save_camera, SIGNAL(clicked(bool)), this->cam, SLOT(save_camera()));
//    connect(ui->gray_camera, SIGNAL(clicked(bool)), this->cam, SLOT(convert2gray()));
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

    QImage imagel;
    QImage imager;

    if(swapFlag){
        imagel = QImage((uchar*)(cam->srcImageL.data), cam->srcImageL.cols, cam->srcImageL.rows, QImage::Format_RGB888);
        imager = QImage((uchar*)(cam->srcImageR.data), cam->srcImageR.cols, cam->srcImageR.rows, QImage::Format_RGB888);
    }else{
        imager = QImage((uchar*)(cam->srcImageL.data), cam->srcImageL.cols, cam->srcImageL.rows, QImage::Format_RGB888);
        imagel = QImage((uchar*)(cam->srcImageR.data), cam->srcImageR.cols, cam->srcImageR.rows, QImage::Format_RGB888);
    }

    QSize image1_size = imagel.size();
    QSize imager_size = imager.size();
    int result_size_width = image1_size.width() + imager_size.width();
    int result_size_height = image1_size.height();

    imagel = imagel.copy(QRect(image_left_crop_left,
                               image_left_crop_up,
                               image1_size.width() - image_left_crop_left,
                               image1_size.height() - image_left_crop_up - image_left_crop_down));
    imagel = imagel.scaled(QSize(image1_size.width(),image1_size.height()),Qt::KeepAspectRatioByExpanding);
    imager = imager.copy(QRect(0,
                               image_right_crop_up,
                               imager_size.width() - image_right_crop_right,
                               imager_size.height() - image_right_crop_up - image_right_crop_down));
    imager = imager.scaled(QSize(imager_size.width(),imager_size.height()),Qt::KeepAspectRatioByExpanding);

    QImage image_result = QImage(result_size_width,result_size_height,QImage::Format_RGB888);
    QPainter painter(&image_result);
    painter.drawImage(0,0,imagel);
    painter.drawImage(image1_size.width(),0,imager);
    painter.end();
    QImage image_result_resize = image_result.scaled(QSize(ui->img_label->width(),ui->img_label->height()));

    ui->img_label->setPixmap(QPixmap::fromImage(image_result_resize));
    ui->img_label->resize(image_result_resize.size());

    ui->img_label->show();
}

void player3D::open_camera()
{
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
}

void player3D::contextMenuEvent(QContextMenuEvent *e)
{
    QCursor cursor = this->cursor();
    QMenu *menu = new QMenu(this);
    QAction *deleteAction = new QAction(tr("close"),this);
    QAction *cameraAction = new QAction(tr("camera"),this);
    menu->addAction(deleteAction);
    menu->addAction(cameraAction);
    connect(deleteAction,SIGNAL(triggered()),SLOT(close()));
    connect(cameraAction,SIGNAL(triggered()),SLOT(open_camera()));
    menu->exec(cursor.pos());
}

void player3D::keyPressEvent(QKeyEvent *e)
{
    switch(e->key()){
    case Qt::Key_A:
        if(e->modifiers() == Qt::ControlModifier){
            image_left_crop_left--;
            image_left_crop_left = image_left_crop_left > 0 ? image_left_crop_left : 0;
        }else{
            image_left_crop_left++;
        }
    case Qt::Key_W:
        image_left_crop_up++;
        break;
    case Qt::Key_S:
        if(e->modifiers() == Qt::ControlModifier)
        {

        }
        image_left_crop_down++;
        break;
    case Qt::Key_E:
        image_right_crop_up++;
        break;
    case Qt::Key_D:
        image_right_crop_down++;
    case Qt::Key_F:
        if(e->modifiers() == Qt::ControlModifier){
            image_right_crop_right--;
            image_right_crop_right = image_right_crop_right > 0 ? image_right_crop_right : 0;
        }else{
            image_right_crop_right++;
        }
        break;
    case Qt::Key_Asterisk:
        swapFlag = !swapFlag;
        break;
    case Qt::Key_BackForward:        
        image_left_crop_up = 0;
        image_left_crop_down = 0;
        image_left_crop_left = 0;
        image_left_crop_right = 0;
        image_right_crop_up = 0;
        image_right_crop_down = 0;
        image_right_crop_left = 0;
        image_right_crop_right = 0;
        break;
    }
}
