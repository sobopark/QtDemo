#ifndef PLAYER3D_H
#define PLAYER3D_H

#include <QMainWindow>

namespace Ui {
class player3D;
}

class player3D : public QMainWindow
{
    Q_OBJECT

public:
    explicit player3D(QWidget *parent = 0);
    ~player3D();

private:
    Ui::player3D *ui;
};

#endif // PLAYER3D_H
