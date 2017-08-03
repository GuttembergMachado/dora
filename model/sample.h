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
public:
    string label;
    string filename;
    string type;
    int size;
    int width;
    int height;
    int depth;

    Mat originalMat;
    Mat grayMat;
    Mat binaryMat;
    Mat xCutMat;
    Mat yCutMat;
    Mat XYCutMat;

    bool load(string filename);
    bool create_grayscale();
    bool create_binary(enumBinarization binMethod);
    bool create_XYCut();
    bool save();
    void saveIntermediate(string folder, bool gray, bool binary, bool xCut, bool yCut, bool XYCut);
    void dump();
};


#endif
