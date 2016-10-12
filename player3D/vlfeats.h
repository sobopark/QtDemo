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
};

#endif // VLFEATS_H
