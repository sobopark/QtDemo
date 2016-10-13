#include "vlfeats.h"
#include <stdio.h>
#include <tchar.h>
#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

using namespace cv;
using namespace std;

vlfeats::vlfeats()
{

}

int vlfeats::getCropPos()
{
   //vl_sift_pix *ImageData=new vl_sift_pix[Image->height*Image->width];
    VL_PRINT ("Hello world!\n") ;
    //vl_sift_
    return 0;

}
int vlfeats::getSift()
{
    Mat img_1 = imread("C:\\Users\\ZhongZY\\Desktop\\000182.bmp", 0);
    int noctaves = 4, nlevels = 2, o_min = 0;

    // noctaves=(int)(log(min)/log(2));
    //在sift.h头文件中有 typedef float vl_sift_pix 声明

    vl_sift_pix *ImageData = new vl_sift_pix[img_1.cols*img_1.rows];
    for (int i = 0; i < img_1.rows; i++)
    {
        uchar *Pixel = img_1.ptr<uchar>(i);
        for (int j = 0; j < img_1.cols; j++)
        {
            ImageData[i*img_1.cols + j] = Pixel[j];
        }
    }
    // 定义VlSiftFilt结构体指针
    VlSiftFilt *SiftFilt = NULL;
    // 创建一个新的sift滤波器
    SiftFilt = vl_sift_new(img_1.cols, img_1.rows, noctaves, nlevels, o_min);

    int KeyPoint = 0;
    int idx = 0;
    if (vl_sift_process_first_octave(SiftFilt, ImageData) != VL_ERR_EOF)
    {
        while (TRUE)
        {
            //计算每组中的关键点
            vl_sift_detect(SiftFilt);
            //遍历并绘制每个点
            KeyPoint += SiftFilt->nkeys;//检测到的关键点的数目
            cout << "关键点数目：" << endl << KeyPoint << endl;
            VlSiftKeypoint *pKeyPoint = SiftFilt->keys;//检测到的关键点
            for (int i = 0; i<SiftFilt->nkeys; i++)
            {
                VlSiftKeypoint TemptKeyPoint = *pKeyPoint;
                pKeyPoint++;
                circle(img_1, cvPoint(TemptKeyPoint.x, TemptKeyPoint.y), TemptKeyPoint.sigma / 2, CV_RGB(255, 0, 0));

                idx++;
                //计算并遍历每个点的方向
                double angles[4];
                int angleCount = vl_sift_calc_keypoint_orientations(SiftFilt, angles, &TemptKeyPoint);//计算关键点的方向
                for (int j = 0; j<angleCount; j++)
                {
                    double TemptAngle = angles[j];
                    printf("%d: %f\n", j, TemptAngle);
                    //计算每个方向的描述
                    float *Descriptors = new float[128];
                    vl_sift_calc_keypoint_descriptor(SiftFilt, Descriptors, &TemptKeyPoint, TemptAngle);
                    int k = 0;
                    while (k<128)
                    {
                        printf("%d: %f", k, Descriptors[k]);
                        printf("; ");
                        k++;
                    }

                    printf("\n");
                    delete[]Descriptors;
                    Descriptors = NULL;
                }
            }
            //下一阶
            if (vl_sift_process_next_octave(SiftFilt) == VL_ERR_EOF)
            {
                break;
            }
            //free(pKeyPoint);
            KeyPoint = NULL;
        }
    }
    vl_sift_delete(SiftFilt);
    delete[]ImageData;
    ImageData = NULL;
    namedWindow("Source Image", 1);
    imshow("Source Image", img_1);

    waitKey(0);
    return 0;
}
void VLMATCH(vl_uint8* L1_pt,vl_uint8* L2_pt, int K1, int K2, double thresh, int* nMatches, double* MATCHES )
{
    int ND = 128;

    pair* pairs_begin = (pair*) malloc(sizeof(pair)*(K1+K2));
    pair* pairs_iterator = pairs_begin ;

    int k1, k2 ;
    const int maxval = 0x7fffffff ;

    for(k1 = 0 ; k1 < K1 ;   k1, L1_pt  = ND )
    {    //kalooo!
        int best = maxval ;
        int second_best = maxval ;
        int bestk = -1 ;
        /* For each point P2[k2] in the second image... */

        for(k2 =  0 ; k2 < K2 ;k2,L2_pt  = ND)
        {
            int bin ;
            int acc = 0 ;
            for(bin = 0 ; bin < ND ;   bin)
            {
                int delta =((int) L1_pt[bin])-((int) L2_pt[bin]) ;
                acc  = delta*delta ;
            }



            /* Filter the best and second best matching point. */

            if(acc < best)
            {
                second_best = best ;
                best = acc ;
                bestk = k2 ;
            }
            else if(acc < second_best)
            {
                second_best = acc ;
            }

        }

        L2_pt -= ND*K2 ;
        /* Lowe's method: accept the match only if unique. */

        if(thresh * (float) best < (float) second_best &&  bestk != -1)
        {
                pairs_iterator->k1 = k1 ;
                pairs_iterator->k2 = bestk ;
                pairs_iterator->score = best ;
                pairs_iterator   ;
                (*nMatches)  ;
        }
    }

    Pair* pairs_end = pairs_iterator ;
    //double* M_pt = (double*)calloc((pairs_end-pairs_begin)*2,sizeof(double));
    double* M_pt = (double*)calloc((*nMatches)*2,sizeof(double));

    //double* M_start = M_pt;

    for(pairs_iterator = pairs_begin ;pairs_iterator < pairs_end;pairs_iterator)
    {
        *M_pt   = pairs_iterator->k1 ;
        *M_pt   = pairs_iterator->k2 ;
    }

    M_pt -= (*nMatches)*2 ;
    memcpy(MATCHES,M_pt,(*nMatches) * 2 * sizeof(double));
    free(pairs_begin) ;
    free(M_pt);
    return;

}
