//
// Guttemberg Machado on 24/07/17.
//

#ifndef SAMPLE_H
#define SAMPLE_H

#include "../tools/helper.h"
#include "../tools/binarization.h"
#include "../tools/xycut.h"
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;
using namespace cv;

class Sample{
    string  mLabel;
    string  mFilename;

    bool createWorkMat(int width, int height);
    bool createGrayscaleMat();
    bool createBinaryMat(enumBinarization binMethod);
    bool createXYCutMat();
    bool saveMat(Mat input, string filename);

public:
    //Constructors
    Sample();

    //Method
    bool load(string filename, string label, bool fixBrokenJPG);
    bool preProcess(int minDimension, int maxDimension, int desiredWidth, int desiredHeight, enumBinarization binMethod);

    //Getters
    string      getFilename();
    string      getLabel();

    //Public properties
    Mat     originalMat;
    Mat     workMat;
    Mat     grayMat;
    Mat     binaryMat;
    Mat     XYCutMat;
};

#endif
