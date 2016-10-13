#ifndef VLFEATS_H
#define VLFEATS_H

#include <vl/generic.h>
#include <vl/stringop.h>
#include <vl/pgm.h>
#include <vl/sift.h>
#include <vl/getopt_long.h>
#include <QtWidgets>
#include <QKeyEvent>
class vlfeats
{
public:
    vlfeats();
    int getCropPos();
    int getSift();
    VLMATCH(vl_uint8* L1_pt,vl_uint8* L2_pt, int K1, int K2, double thresh, int* nMatches, double* MATCHES );
};

#endif // VLFEATS_H
