//
// Created by gutto on 24/07/17.
//

#ifndef SAMPLE_H
#define SAMPLE_H

#include "../tools/helper.h"
#include "../tools/binarization.h"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;
using namespace cv;

class Sample{
private:
    void fixJPEG(string filename);
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

    bool load(string filename);
    bool create_grayscale();
    bool create_binary(enumBinarization binMethod);
    bool save(bool original, bool gray, bool binary, bool xCut, bool yCut);
    void dump();

//public:

    //Mat		originalMat;
    //Mat		binaryMat;
    //Mat		verticalProjectionMat;
    //Mat		horizontalProjectionMat;
    //Mat		projectionMat;
    //bool loadFile(string fileName);
    //void	saveBinaryFile(string fileName);
    //void	saveProjectionFile(string fileName);

};


#endif //SAMPLE_H
