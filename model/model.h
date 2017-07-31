//
// Guttemberg Machado on 24/07/17.
//
#ifndef MODELS_H
#define MODELS_H

#include <opencv2/features2d.hpp>  //FeatureDetector, DescriptorExtractor, BOWTrainer, DescriptorMatcher
#include <opencv2/xfeatures2d/nonfree.hpp>   //SiftFeatureDetector, SiftDescriptorExtractor, SurfFeatureDetector, SurfDescriptorExtractor

#include "../tools/helper.h"
#include "sample.h"

using namespace std;
using namespace cv;
using namespace xfeatures2d;
enum enumModels
{
	model_PROJECTION = 0,
	model_BAG_OF_FEATURES = 1,
};

enum enumFeatures
{
	feature_SIFT = 0,	//Scale Invariant Feature Transform (* patented)
	feature_SURF = 1,   //Speeded Up Robust Features (* patented)
	feature_LBP = 2,    //Local Binary Patterns
	feature_FAST = 3,   //Feature From Accelerated Segment Tests
	feature_BRIEF = 4,  //Binary Robust Independent Elementary Features
	feature_ORB = 5,    //Oriented Fast & Rotated BRIEF
    feature_START = 6,  //
    feature_MSER = 7,   //
    feature_GFTT = 8,   //Good Features To Track Detector
    feature_HARRIS = 9, //Good Features To Track Detector with HARRIS detector
    feature_DENSE = 10, //
    feature_BLOB = 11,  //
};

enum enumMatchers
{
	matcher_BRUTE_FORCE = 0,
	matcher_FLANN = 1,					// Fast Library for Approximating Nearest Neighbors
	matcher_K_MEANS_CLUSTERING = 2,
};

class Model {
    vector<Sample>	            samples;
	bool                        initializeModules();
	bool                        initializeDictionary();
    Ptr<FeatureDetector>        detector;
    Ptr<DescriptorExtractor>    extractor;
    Ptr<BOWTrainer>	            trainer;
    Ptr<DescriptorMatcher>      matcher;
	Mat							dictionary;
    int					        dictionarySize;
public:
	enumModels       classificationModel;
	enumFeatures     featureType;
    enumMatchers     matcherModel;
    enumBinarization binarizationMethod;
    string           getModelName();
    string           getFeatureName();
    string           getMatcherName();
    string           getBinarizationName();
	string 			 name;
	string 			 filename;
	string 			 folder;
	bool             create(string sampleFolder);
	bool             load();
	bool             save();
};
#endif
