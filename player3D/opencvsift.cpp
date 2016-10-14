#include "opencvsift.h"

using namespace cv;
using namespace std;

opencvsift::opencvsift()
{

}

int opencvsift::siftDtc()
{
    Mat img=imread("C:\\Users\\ZhongZY\\Desktop\\000182.bmp");
    SiftFeatureDetector  siftdtc;
    vector<KeyPoint> kp;
    siftdtc.detect(img,kp);
    Mat outimg;
    drawKeypoints(img,kp,outimg);
    imshow("image1 keypoints",outimg);
    //KeyPoint kp_;
    //vector<KeyPoint>::iterator itvc;
    //for(itvc=kp.begin();itvc!=kp.end();itvc++)
    //{
    //    cout<<"angle:"<<itvc->angle<<"\t"<<itvc->class_id<<"\t"<<itvc->octave<<"\t"<<itvc->pt<<"\t"<<itvc->response<<endl;
    //}
    return 0;
}
