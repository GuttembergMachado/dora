#ifndef MODELS_H
#define MODELS_H

#include <string>
#include <vector>

#include "../tools/helper.h"

//#include "opencv2/highgui/highgui.hpp"
//#include "opencv2/imgproc/imgproc.hpp"

//#include <Windows.h>
//#include "log.h"
//#include "helper.h"
//#include "document.h"
//#include "preprocessor.h"
//#include "xycut.h"

using namespace std;

//class Sample{
//public:
//	Sample(string sample_label, string sample_filename, Mat sample_XCut, Mat sample_YCut );
//	string			label;
//	string			filename;
//	Mat				xCut;
//	Mat				yCut;
//};

enum enumModels
{
	model_PROJECTION = 0,
	model_BAG_OF_FEATURES = 1,
};

enum enumFeatures
{
	feature_SIFT = 0,	//Scale Invariant Feature Transform				
	feature_SURF = 1,   //Speeded Up Robust Features
	feature_LBP = 2,    //Local Binary Patters
	feature_FAST = 3,   //Feature From Accelerated Segment Tests
	feature_BRIEF = 4,  //Binary Robust Independent Elementary Features
	feature_ORB = 5,    //Oriented Fast & Rotated BRIEF
};

enum enumMatching
{
	matching_BRUTE_FORCE = 0,
	matching_FLANN = 1,					// Fast Library for Approximating Nearest Neighbors
	matching_K_MEANS_CLUSTERING = 2,
};

class Model {
public:
	string source;
	string filename;
	enumModels classificationModel;
	enumFeatures featureType;
	enumMatching matchingModel;

	bool createModel();

	bool loadModel();

	bool saveModel();

	void dumpModel();

};
#endif
