#ifndef OPENCVSIFT_H
#define OPENCVSIFT_H

#include <opencv2/opencv.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/nonfree/nonfree.hpp>
#include <opencv2/legacy/legacy.hpp>
#include <vector>

using namespace cv;
class opencvsift
{
public:
    opencvsift();
    vector<KeyPoint> siftDtc(Mat img);
    Mat siftMat(Mat img,vector<KeyPoint> kp);
    vector<DMatch> siftMatch(Mat img_left,Mat img_right);
    //static bool comparison(DMatch a,DMatch b);
    int getXpox(vector<DMatch> Matchs, vector<KeyPoint> kp_left, vector<KeyPoint> kp_right, int eps);
};

#endif // OPENCVSIFT_H
