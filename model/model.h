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
#include "class.h"

using namespace std;
using namespace cv;
using namespace xfeatures2d;
using namespace ml;

enum enumClassifier
{
	model_PROJECTION = 0,
	model_BAG_OF_FEATURES = 1,
};

enum enumFeature
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

enum enumMatcher
{
	matcher_BRUTE_FORCE = 0,
	matcher_FLANN = 1,
	matcher_K_MEANS_CLUSTERING = 2,
};


class Model {

    //methods
    bool                        loadSamples(string sampleFolder);
    bool 						preProcessSamples();
	bool             			createDictionary();
	bool 						prepareTrainingSet();

    Ptr<FeatureDetector>        detector;
    Ptr<DescriptorExtractor>    extractor;
    Ptr<BOWTrainer>	            trainer;
    Ptr<DescriptorMatcher>      matcher;
    Ptr<SVM>                    svm;
    Mat							mDictionary;
    Mat							mTrainingData;
    Mat							mTrainingLabel;

    //logging helper routines
    string                      getClassifierName();
    string                      getFeatureName();
    string                      getMatcherName();
    string                      getBinarizationName();

    //properties
    vector<Class>               mClasses;
    string                      mFilename;
    int					        mDictionarySize = 1000;
    int 						mMinDimension = 70;
    int 						mMaxDimension = 720;
    enumFeature                 mFeatureType;
    enumMatcher                 mMatcherType;
    enumClassifier              mClassifierType;
    enumBinarization            mBinarizationType;
    long                        mAverageSampleWidth = 0;
    long                        mAverageSampleHeight = 0;
    bool                        mSaveTempFiles = true;

public:

    //methods
    bool             initialize();
    bool             create(string sampleFolder);
    bool             load();
    bool             save();

    //setters
    void             setClassifierType(enumClassifier type);
    void             setFeatureType(enumFeature type);
    void             setMatcherType(enumMatcher type);
    void             setBinarizationType(enumBinarization type);
    void             setFilename(string filename);

    //getters
    string           getFilename();

	//string 		   name;
	//string 		   folder;
	//bool			   saveIntermediateFiles;
	//bool             create(string sampleFolder);
    //bool             classify(string path);

};

#endif
