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
    //string  mType;
    //int     mSize;
    //int     mWidth;
    //int     mHeight;
    //int     mDepth;
    Mat     mOriginalMat;

public:
    //Constructors
    Sample();

    //Method
    bool load(string filename, string label, bool fixBrokenJPG);
    bool save();

    //Getters
    string getFilename();
    string getLabel();
    int getWidth();
    int getHeight();

    //string getType();
    //int getSize();
    //int getDepth();
    //Public variables:
    //bool preProcess();
    //bool extractFeatures();
    //Mat workMat;
    //Mat grayMat;
    //Mat binaryMat;
    //Mat xCutMat;
    //Mat yCutMat;
    //Mat XYCutMat;
    //bool createWorkMat(int width, int height);
    //bool createGrayscaleMat();
    //bool createBinaryMat(enumBinarization binMethod);
    //bool createXYCutMat();
    //void saveIntermediate(string folder, bool gray, bool binary, bool xCut, bool yCut, bool XYCut);
    //void dump();

};

#endif
