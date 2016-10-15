#include "player3d.h"
#include "ui_player3d.h"
#include "camera.h"
#include "opencvsift.h"

#include <QtWidgets>
#include <QKeyEvent>

// version_1_4 : 2016_10_12_15_26

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
    recordFlag = false;
    image_left_mirror_flag = false;
    image_right_mirror_flag = false;
    image_mirror_flag = false;

    image_left_crop_up = 0;
    image_left_crop_down = 0;
    image_left_crop_left = 0;
    image_left_crop_right = 0;
    image_right_crop_up = 0;
    image_right_crop_down = 0;
    image_right_crop_left = 0;
    image_right_crop_right = 0;

    record_path = "";
    record_image_count = 0;

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

int player3D::autoGetpos()
{
    opencvsift siftTool;
    Mat img_left = cam->srcImageL;
    Mat img_right= cam->srcImageR;
    int eps=img_left.size().width;
    int pos=0;
    vector<KeyPoint> kp_left,kp_right;
    vector<DMatch> result;
    vector<KeyPoint> del_1,del_2;
    vector<DMatch> del_3;
    Mat des_left,des_right;
    kp_left = siftTool.siftDtc(img_left);
    kp_right= siftTool.siftDtc(img_right);
    des_left = siftTool.siftMat(img_left,kp_left);
    des_right= siftTool.siftMat(img_right,kp_right);
    result=siftTool.siftMatch(des_left,des_right);
    pos=siftTool.getXpox(result,kp_left,kp_right,eps);
    kp_left.swap(del_1);
    kp_right.swap(del_2);
    result.swap(del_3);
    return pos;
    //return 0;
}

void player3D::updateImage()
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

    if(image_left_mirror_flag){
        imagel = imagel.mirrored(true,false);
    }

    if(image_right_mirror_flag){
        imager = imager.mirrored(true,false);
    }

    QSize image1_size = imagel.size();
    QSize imager_size = imager.size();
    int result_size_width = image1_size.width() + imager_size.width();
    int result_size_height = image1_size.height();

    imagel = imagel.copy(QRect(image_left_crop_left,
                               image_left_crop_up,
                               image1_size.width() - image_left_crop_left - image_left_crop_right,
                               image1_size.height() - image_left_crop_up - image_left_crop_down));
    imagel = imagel.scaled(QSize(image1_size.width(),image1_size.height()),Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
    imager = imager.copy(QRect(image_right_crop_left,
                               image_right_crop_up,
                               imager_size.width() - image_right_crop_left - image_right_crop_right,
                               imager_size.height() - image_right_crop_up - image_right_crop_down));
    imager = imager.scaled(QSize(imager_size.width(),imager_size.height()),Qt::IgnoreAspectRatio,Qt::SmoothTransformation);

    QImage image_result = QImage(result_size_width,result_size_height,QImage::Format_RGB888);
    QPainter painter(&image_result);
    painter.drawImage(0,0,imagel);
    painter.drawImage(image1_size.width(),0,imager);
    painter.end();

    if(image_mirror_flag){
        image_result = image_result.mirrored(true,false);
    }

    if(recordFlag){

        QString s = QString("%1").arg(record_image_count, 4, 10, QChar('0'));
        QString record_image_path = record_path + "\\" + s + ".png";
        image_result.save(record_image_path);
        record_image_count++;

    }

    QImage image_result_resize = image_result.scaled(QSize(ui->img_label->width(),ui->img_label->height()),Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
    ui->img_label->setPixmap(QPixmap::fromImage(image_result_resize));
//    ui->img_label->resize(image_result_resize.size());
    ui->img_label->show();
}

void player3D::open_camera()
{
    if(cam->videoCapL.open(0))
    {
        cam->videoCapL.set(CV_CAP_PROP_FRAME_WIDTH,1080);
        cam->videoCapL.set(CV_CAP_PROP_FRAME_HEIGHT,960);
        cam->srcImageL = Mat::zeros(cam->videoCapL.get(CV_CAP_PROP_FRAME_HEIGHT), cam->videoCapL.get(CV_CAP_PROP_FRAME_WIDTH), CV_8UC3);
        if(cam->videoCapR.open(1))
        {
            cam->videoCapR.set(CV_CAP_PROP_FRAME_WIDTH,1080);
            cam->videoCapR.set(CV_CAP_PROP_FRAME_HEIGHT,960);
            cam->srcImageR = Mat::zeros(cam->videoCapR.get(CV_CAP_PROP_FRAME_HEIGHT), cam->videoCapR.get(CV_CAP_PROP_FRAME_WIDTH), CV_8UC3);
            theTimer.start(33);
        }
    }
}

void player3D::close_camera()
{
    theTimer.stop();
}

void player3D::record()
{
    QFileDialog::Options options = QFileDialog::DontResolveSymlinks | QFileDialog::ShowDirsOnly;
                         options |= QFileDialog::DontUseNativeDialog;
    record_path = QFileDialog::getExistingDirectory(this,
                                    tr("Open Directory"),
                                    ".",
                                    options);
    if (!record_path.isEmpty()){
        recordFlag = true;
        record_image_count = 1;
    }
}


void player3D::contextMenuEvent(QContextMenuEvent *e)
{
    QCursor cursor = this->cursor();
    QMenu *menu = new QMenu(this);
    QAction *deleteAction = new QAction(tr("close"),this);
    QAction *cameraAction = new QAction(tr("camera"),this);
    QAction *recordAction = new QAction(tr("record"),this);
    menu->addAction(deleteAction);
    menu->addAction(cameraAction);
    menu->addAction(recordAction);
    connect(deleteAction,SIGNAL(triggered()),qApp,SLOT(quit()));
    connect(cameraAction,SIGNAL(triggered()),SLOT(open_camera()));
    connect(recordAction,SIGNAL(triggered()),SLOT(record()));
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
        break;
    case Qt::Key_W:
        if(e->modifiers() == Qt::ControlModifier){
            image_left_crop_up--;
            image_left_crop_up = image_left_crop_up > 0 ? image_left_crop_up : 0;
        }else{
            image_left_crop_up++;
        }
        break;
    case Qt::Key_S:
        if(e->modifiers() == Qt::ControlModifier){
            image_left_crop_down--;
            image_left_crop_down = image_left_crop_down > 0 ? image_left_crop_down : 0;
        }else{
            image_left_crop_down++;
        }
        break;
    case Qt::Key_D:
        if(e->modifiers() == Qt::ControlModifier){
            image_left_crop_right--;
            image_left_crop_right = image_left_crop_right > 0 ? image_left_crop_right : 0;
        }else{
            image_left_crop_right++;
        }
        break;
    case Qt::Key_H:
        if(e->modifiers() == Qt::ControlModifier){
            image_right_crop_left--;
            image_right_crop_left = image_right_crop_left > 0 ? image_right_crop_left : 0;
        }else{
            image_right_crop_left++;
        }
        break;
    case Qt::Key_U:
        if(e->modifiers() == Qt::ControlModifier){
            image_right_crop_up--;
            image_right_crop_up = image_right_crop_up > 0 ? image_right_crop_up : 0;
        }else{
            image_right_crop_up++;
        }
        break;
    case Qt::Key_J:
        if(e->modifiers() == Qt::ControlModifier){
            image_right_crop_down--;
            image_right_crop_down = image_right_crop_down > 0 ? image_right_crop_down : 0;
        }else{
            image_right_crop_down++;
        }
        break;
    case Qt::Key_K:
        if(e->modifiers() == Qt::ControlModifier){
            image_right_crop_right--;
            image_right_crop_right = image_right_crop_right > 0 ? image_right_crop_right : 0;
        }else{
            image_right_crop_right++;
        }
        break;
    case Qt::Key_X:
        image_left_mirror_flag = !image_left_mirror_flag;
        break;
    case Qt::Key_M:
        image_right_mirror_flag = !image_right_mirror_flag;
        break;
    case Qt::Key_B:
        image_mirror_flag = !image_mirror_flag;
        break;
    case Qt::Key_Asterisk:
        swapFlag = !swapFlag;
        break;
    case Qt::Key_O:
        image_left_crop_up = 0;
        image_left_crop_down = 0;
        image_left_crop_left = 0;
        image_left_crop_right = 0;
        image_right_crop_up = 0;
        image_right_crop_down = 0;
        image_right_crop_left = 0;
        image_right_crop_right = 0;
        break;
    case Qt::Key_Z:
        int tmp=autoGetpos();
        image_left_crop_left=tmp;
        image_right_crop_right=tmp;

    }
}
