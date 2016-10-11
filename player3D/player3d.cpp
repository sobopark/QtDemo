#include "player3d.h"
#include "ui_player3d.h"


// test github push

player3D::player3D(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::player3D)
{
    ui->setupUi(this);
}

player3D::~player3D()
{
    delete ui;
}
