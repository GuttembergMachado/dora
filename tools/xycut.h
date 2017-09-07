//
// Guttemberg Machado on 31/07/17.
//

#ifndef DORA_XYCUT_H
#define DORA_XYCUT_H

#include "helper.h"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"

using namespace cv;
using namespace std;

bool getXCut(Mat &source, Mat &dest);
bool getYCut(Mat &source, Mat &dest);
bool getXYCut(Mat &source, Mat &dest);

#endif

