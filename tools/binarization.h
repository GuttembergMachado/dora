//
// Guttemberg Machado on 24/07/17.
//
#ifndef BINARIZATION_H
#define BINARIZATION_H

#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;
using namespace std;

enum enumBinarization
{
    binarization_TRESHOLD = 0, //thresold is 127
    binarization_MEAN = 1,     //treshold is "the mean of neighbourhood area"
    binarization_GAUSSIAN =2,  //threshold is weighted sum of neighbourhood values where weights are a gaussian window
    binarization_NIBLACK = 10,
    binarization_SAUVOLA = 11,
    binarization_WOLFJOLION = 12,
    binarization_BRADLEY = 13,
    binarization_CLAHE = 14,
};

#define uget(x,y)	 at<unsigned char>(y,x)
#define uset(x,y,v)  at<unsigned char>(y,x)=v;
#define fget(x,y)	 at<float>(y,x)
#define fset(x,y,v)  at<float>(y,x)=v;

bool binarize(Mat source, Mat dest, enumBinarization method);

#endif