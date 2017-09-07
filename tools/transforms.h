//
// Created by gutto on 06/09/17.
//

#ifndef DORA_TRANSFORMS_H
#define DORA_TRANSFORMS_H

#include "helper.h"
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;
using namespace cv;

void deskewMat(Mat &inputMat, Mat &outputMat, Point A, Point B, Point C, Point D);
void detectBorder(Mat &inputMat, Mat &outputMat);

#endif