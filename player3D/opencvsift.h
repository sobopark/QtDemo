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
    int siftDtc();
};

#endif // OPENCVSIFT_H
