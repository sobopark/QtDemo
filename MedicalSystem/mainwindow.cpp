#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initUI();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initUI()
{
    QString strPath = QCoreApplication::applicationDirPath();
    qDebug()<<strPath;
    QString strCssFile = strPath + "/default.css";
    QFile fCss(strCssFile);
    if(!fCss.open(QFile::ReadOnly))
    {
        qDebug("css File %s load false.",strCssFile);
        return;
    }
    QString strCssContent(fCss.readAll());
    setStyleSheet(strCssContent);
    fCss.close();
}
