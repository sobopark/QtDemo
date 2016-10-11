#-------------------------------------------------
#
# Project created by QtCreator 2016-10-10T13:32:04
#
#-------------------------------------------------

QT       += core gui\
            multimedia\
            multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = player3D
TEMPLATE = app


SOURCES += main.cpp\
        player3d.cpp \
        camera.cpp

INCLUDEPATH += E:\work\Library\OpenCV\OpenCV2.4.11\opencv\build\include \
               E:\work\Library\OpenCV\OpenCV2.4.11\opencv\build\include\opencv \
               E:\work\Library\OpenCV\OpenCV2.4.11\opencv\build\include\opencv2

LIBS += E:\work\Library\OpenCV\OpenCV2.4.11\opencv\build\x86\vc12\lib\opencv_highgui2411d.lib \
        E:\work\Library\OpenCV\OpenCV2.4.11\opencv\build\x86\vc12\lib\opencv_core2411d.lib  \
        E:\work\Library\OpenCV\OpenCV2.4.11\opencv\build\x86\vc12\lib\opencv_imgproc2411d.lib\
        E:\work\Library\OpenCV\OpenCV2.4.11\opencv\build\x86\vc12\lib\opencv_calib3d2411d.lib\
        E:\work\Library\OpenCV\OpenCV2.4.11\opencv\build\x86\vc12\lib\opencv_contrib2411d.lib\
        E:\work\Library\OpenCV\OpenCV2.4.11\opencv\build\x86\vc12\lib\opencv_features2d2411d.lib\
        E:\work\Library\OpenCV\OpenCV2.4.11\opencv\build\x86\vc12\lib\opencv_flann2411d.lib\
        E:\work\Library\OpenCV\OpenCV2.4.11\opencv\build\x86\vc12\lib\opencv_gpu2411d.lib\
        E:\work\Library\OpenCV\OpenCV2.4.11\opencv\build\x86\vc12\lib\opencv_highgui2411d.lib\
        E:\work\Library\OpenCV\OpenCV2.4.11\opencv\build\x86\vc12\lib\opencv_legacy2411d.lib\
        E:\work\Library\OpenCV\OpenCV2.4.11\opencv\build\x86\vc12\lib\opencv_ml2411d.lib\
        E:\work\Library\OpenCV\OpenCV2.4.11\opencv\build\x86\vc12\lib\opencv_objdetect2411d.lib\
        E:\work\Library\OpenCV\OpenCV2.4.11\opencv\build\x86\vc12\lib\opencv_ts2411d.lib\
        E:\work\Library\OpenCV\OpenCV2.4.11\opencv\build\x86\vc12\lib\opencv_video2411d.lib

HEADERS  += player3d.h \
            camera.h

FORMS    += player3d.ui
