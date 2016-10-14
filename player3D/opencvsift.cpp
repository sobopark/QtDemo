#include "opencvsift.h"

using namespace cv;
using namespace std;

opencvsift::opencvsift()
{

}


vector<KeyPoint> opencvsift::siftDtc(Mat img)
{

    vector<KeyPoint>  kp;
    SiftFeatureDetector  siftdtc;
    siftdtc.detect(img,kp);
    return kp;

}
Mat opencvsift::siftMat(Mat img,vector<KeyPoint> kp)
{
    SiftDescriptorExtractor extractor;
    Mat descriptor;
    extractor.compute(img,kp,descriptor);
    return descriptor;
}

vector<DMatch> opencvsift::siftMatch(Mat img_left,Mat img_right)
{
    BruteForceMatcher<L2<float>> matcher;
    vector<DMatch> matches;
    //Mat img_matches;
    matcher.match(img_left,img_right,matches);
    //drawMatches(img,kp1,img2,kp2,matches,img_matches);
    //imshow("matches",img_matches);
    return matches;
}

int opencvsift::getXpox(vector<DMatch> Matchs,vector<KeyPoint> kp_left,vector<KeyPoint> kp_right,int eps)
{
    struct tmp
    {
       bool operator () (DMatch a,DMatch b)
       {
            return a.distance<b.distance;
       }
    } obj;

    sort(Matchs.begin(), Matchs.end(), obj);

    int left_pos ,right_pos;
    Point left_pt,right_pt;
    int sum_diff=0,num_diff=0,tmp_diff=0;
    for (int i=0;i<Matchs.size()/3;i++)
    {

        {

           left_pos = Matchs[i].queryIdx;
           right_pos= Matchs[i].trainIdx;
           left_pt = kp_left[left_pos].pt;
           right_pt= kp_right[right_pos].pt;
           tmp_diff=left_pt.x-right_pt.x;
           if (tmp_diff>=0 && tmp_diff<=eps)
           {
               sum_diff=sum_diff+tmp_diff;
               num_diff++;
           }
        }
    }
    int result_diff=sum_diff/num_diff;

    return result_diff;

}



