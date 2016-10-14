#include "opencvsift.h"

using namespace cv;
using namespace std;

opencvsift::opencvsift()
{

}

vector<KeyPoint> opencvsift::siftDtc(Mat img)
{
    //Mat img=imread("C:\\Users\\ZhongZY\\Desktop\\000182.bmp");
    vector<KeyPoint>  kp;
    SiftFeatureDetector  siftdtc;
    siftdtc.detect(img,kp);
    //kp.clear();
    //return descriptor1;
    return kp;

}
Mat opencvsift::siftMat(Mat img,vector<KeyPoint> kp)
{
    SiftDescriptorExtractor extractor;
    Mat descriptor;
    extractor.compute(img,kp,descriptor);
    return descriptor;
}
