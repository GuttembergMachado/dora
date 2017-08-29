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

enum enumRescale
{
    rescale_CROP = 0,
    rescale_SCALE = 1,
    rescale_FIT = 2
};

class Sample{
    string mLabel;
    string mFilename;
    string mTemporaryFolder;

    bool createWorkMat(int desiredDimension, enumRescale rescaleMethod);
    bool createGrayscaleMat();
    bool createBinaryMat(enumBinarization binMethod);
    bool createXYCutMat();
    bool saveMat(Mat input, string filename);

public:
    //Constructors
    Sample();

    //Method
    bool load(string filename, string label, bool fixBrokenJPG);
    bool preProcess(int desiredDimension, enumRescale rescaleMethod, enumBinarization binMethod);

    //Getters
    string      getFilename();
    string      getLabel();

    //Setter
    void setTemporaryFolder(string folder);

    //Public properties
    Mat     originalMat;
    Mat     workMat;
    Mat     grayMat;
    Mat     binaryMat;
    Mat     XYCutMat;

    vector<KeyPoint> features;
    Mat              dic_descriptors;
    Mat              bow_descriptors;

};

#endif
