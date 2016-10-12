#ifndef PLAYER3D_H
#define PLAYER3D_H

#include <QMainWindow>

#include "camera.h"
#include "ui_player3d.h"

namespace Ui {
class player3D;
}

class player3D : public QMainWindow
{
    Q_OBJECT

public:
    explicit player3D(QWidget *parent = 0);
    ~player3D();

    Camera *cam;

public slots:
    void updateImage();
    void open_camera();
    void close_camera();
    void record();

private:
    Ui::player3D *ui;

    QTimer theTimer;
    bool swapFlag;
    bool recordFlag;

    int image_left_crop_up;
    int image_left_crop_down;
    int image_left_crop_left;
    int image_left_crop_right;
    int image_right_crop_up;
    int image_right_crop_down;
    int image_right_crop_left;
    int image_right_crop_right;
    int record_image_count;

    QString record_path;

protected:
    void paintEvent(QPaintEvent *e);
    void contextMenuEvent(QContextMenuEvent *e) Q_DECL_OVERRIDE;
    void keyPressEvent(QKeyEvent *e);
};

#endif // PLAYER3D_H
