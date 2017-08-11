//
// Guttemberg Machado on 24/07/17.
//

#ifndef MODELS_H
#define MODELS_H

#include <opencv2/features2d.hpp>  //FeatureDetector, DescriptorExtractor, BOWTrainer, DescriptorMatcher
#include <opencv2/xfeatures2d/nonfree.hpp>   //SiftFeatureDetector, SiftDescriptorExtractor, SurfFeatureDetector, SurfDescriptorExtractor
#include <opencv2/ml.hpp> //SVM

#include "../tools/helper.h"
#include "sample.h"

using namespace std;
using namespace cv;
using namespace xfeatures2d;
using namespace ml;

enum enumModels
{
	model_PROJECTION = 0,
	model_BAG_OF_FEATURES = 1,
};

enum enumFeatures
{
	feature_SIFT = 0,
	feature_SURF = 1,
	feature_LBP = 2,
	feature_FAST = 3,
	feature_BRIEF = 4,
	feature_ORB = 5,
    feature_START = 6,
    feature_MSER = 7,
    feature_GFTT = 8,
    feature_HARRIS = 9,
    feature_DENSE = 10,
    feature_BLOB = 11,
};

enum enumMatchers
{
	matcher_BRUTE_FORCE = 0,
	matcher_FLANN = 1,
	matcher_K_MEANS_CLUSTERING = 2,
};

class Class{
public:
	string label;
	int count;
	int width;
	int height;;
};

class Model {
    vector<Sample>	            samples;
	bool             			createDictionary();
	bool 						prepareTrainingSet();
    bool 						preProcessSamples();
    Ptr<FeatureDetector>        detector;
    Ptr<DescriptorExtractor>    extractor;
    Ptr<BOWTrainer>	            trainer;
    Ptr<DescriptorMatcher>      matcher;
    Ptr<SVM>                    svm;
	Mat							dictionary;
    int					        dictionarySize = 1500;
	int 						minDimension = 70;
	int 						maxDimension = 720;
	int 						averageDimension = 200;
	Mat							trainingData;
	Mat							trainingLabel;
    string                      getMatType(Mat m);
	void 						updateClass(Sample s);
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
	bool			 saveIntermediateFiles;
	bool             create(string sampleFolder);
	bool             load();
	bool             save();
	bool             initialize();
    bool             classify(string path);
	vector<Class>	 sampleClass;
};

#endif
