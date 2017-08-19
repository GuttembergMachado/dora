//
// Guttemberg Machado on 24/07/17.
//
//      -----------------------+------------------------------+
//      |      TRAINING:       |        CLASSIFYING           |
//      +----------------------+------------------------------+
//      | List files           | List file or folder          |
//      +----------------------+------------------------------+
//      | Load Samples         |                              |
//      | Pre Process Samples  |                              |
//      +----------------------+------------------------------+
//      | Iterate Samples      | Load dictionary from file    |
//      |    Extract features  |                              |
//      | Create Dictionary    |                              |
//      +----------------------+------------------------------+
//      | Iterate Samples      | Load labels from file        |
//      |    Create Labels     |                              |
//      +----------------------+------------------------------+
//      | Iterate Samples      | Load training set from file  |
//      |    Extract features  |                              |
//      | Create Training Set  |                              |
//      +----------------------+------------------------------+
//      | Train                | Predict                      |
//      +----------------------+------------------------------+

#include "model.h"

bool Model::create(string sampleFolder){

    int64 startTask = getTick();
    int64 startSubtask;
    bool res = false;

	try{
        Log(log_Error, "model.cpp", "create", "   Creating model...");

        if(loadSamples(sampleFolder)){

            if(preProcessSamples()){

                if(createDictionary()){

                    if(prepareTrainingSet()){

                        Log(log_Debug, "model.cpp", "create", "   Training the SVM...");
                        startSubtask = getTick();
                        res = mSupportVectorMachine->train(mTrainingData, ROW_SAMPLE, mTrainingLabel);  //(ROW_SAMPLE: each training sample is a row of samples; COL_SAMPLE :each training sample occupies a column of samples)
                        Log(log_Debug, "model.cpp", "create", "      Done. Training took %s seconds.", getDiffString(startSubtask).c_str());

                    }
                }
            }
        }

        if(res)
            Log(log_Debug, "model.cpp", "create", "   Done. Creating model took %s seconds.", getDiffString(startTask).c_str());
        else
            Log(log_Error, "model.cpp", "create", "   Done. Creating model failed after %s seconds!", getDiffString(startTask).c_str());

        return res;

    }catch(const std::exception& e){
        Log(log_Error, "model.cpp", "create",  "   Error creating model: %s", e.what()) ;
    }

    return false;

}

bool Model::load(){

    try{

        if (isFile(mFilename)){
            Log(log_Debug, "model.cpp", "load", "   Loading model file...");

            //Reads the model
            //svm.load(filename.c_str());
            string trainingSetFile = mFilename + "_trainingSetFile.xml";

            if (isFile(trainingSetFile)){

                FileStorage fs(trainingSetFile, FileStorage::READ);
                fs["dictionary"] >> mDictionary;
                //fs["labels"] >> labels;
                fs.release();

                Log(log_Debug, "model.cpp", "loadDictionary", "      Loading existing dictionary...");


                //FileStorage fs(trainingSetFile, FileStorage::READ);
                //f//s["dictionary"] >> dictionary;
                //fs["trainingData"] >> trainingData;
                //fs["labels"] >> labels;
                //fs["trainingLabel"] >> trainingLabel;
                //fs.release();

                //Lê o modelo
                //svm.load(filename.c_str());


                Log(log_Debug, "model.cpp", "load", "      TODO: (NOT IMPLEMENTED): Model was loaded.");
                return true;

            }else{
                Log(log_Error, "model.cpp", "loadModel", "      Dictionary file was not found.");
            }
        }
        else{
            Log(log_Error, "model.cpp", "load", "      Model file was not found.");
        }

    }catch(const std::exception& e){
        Log(log_Error, "model.cpp", "load", "      Failed to load model: %s", e.what() ) ;
    }

    return false;
}

bool Model::save(){

    int64 startTask = getTick();
    int64 startSubtask;

	Log(log_Debug, "model.cpp", "save", "   Saving model files...");

	try{

        string folder = getFolderName(mFilename);
        string dictionaryFilename = folder + "/dictionary.yml";
        string trainingDataFilename = folder + "/trainingData.yml";
        string trainingLabelFilename = folder + "/trainingLabel.yml";

        startSubtask = getTick();
        Log(log_Debug, "model.cpp", "save", "      Saving dictionary on file '%s'...", dictionaryFilename.c_str() );
        FileStorage dictionaryFile(dictionaryFilename.c_str(), FileStorage::WRITE);
        dictionaryFile << "dictionary" << mDictionary;
        dictionaryFile.release();
        Log(log_Debug, "model.cpp", "save", "         Done saving dictionary file in %s seconds.",  getDiffString(startSubtask).c_str());

        startSubtask = getTick();
        Log(log_Debug, "model.cpp", "save", "      Saving training data on '%s'...", trainingDataFilename.c_str());
        FileStorage trainingDataFile(trainingDataFilename.c_str(), FileStorage::WRITE);
        trainingDataFile << "trainingData" << mDictionary;
        trainingDataFile.release();
        Log(log_Debug, "model.cpp", "save", "         Done saving training data in %s seconds.",  getDiffString(startSubtask).c_str());

        startSubtask = getTick();
        Log(log_Debug, "model.cpp", "save", "      Saving training data on file '%s'...", trainingLabelFilename.c_str());
        FileStorage trainingLabelFile(trainingLabelFilename.c_str(), FileStorage::WRITE);
        trainingLabelFile << "trainingLabel" << mDictionary;
        trainingLabelFile.release();
        Log(log_Debug, "model.cpp", "save", "         Done saving training data in %s seconds.",  getDiffString(startSubtask).c_str());

        startSubtask = getTick();
        Log(log_Debug, "model.cpp", "save", "      Saving model to file '%s'...",  mFilename.c_str());
        mSupportVectorMachine->save(mFilename.c_str());
        Log(log_Debug, "model.cpp", "save", "         Done saving model took %s seconds.",  getDiffString(startSubtask).c_str());

        Log(log_Debug, "model.cpp", "save", "      Done. Saving files took %s seconds.",  getDiffString(startSubtask).c_str());

		return true;

	}catch(const std::exception& e){
		Log(log_Error, "model.cpp", "save", "      Failed to save model: %s", e.what() ) ;
	}

	return false;
}

bool Model::initialize(){

    int64 startTask = getTick();
    int64 startSubtask;

    try{
        Log(log_Error, "model.cpp", "initialize", "   Initializing modules...");
        Log(log_Error, "model.cpp", "initialize", "      Preset dictionary size is %i.", mDictionarySize);
        Log(log_Error, "model.cpp", "initialize", "      Preset minimum sample dimension is %i.", mMinDimension);
        Log(log_Error, "model.cpp", "initialize", "      Preset maximum sample dimension is %i.", mMaxDimension);

        mTrainingData  = Mat(0,mDictionarySize, CV_32S);
        mTrainingLabel = Mat(0, 1, CV_32S);

        Log(log_Error, "model.cpp", "initialize", "      Initializing feature detector module: '" + getFeatureName() + "'...");
        switch (mFeatureType)
        {
            case feature_SIFT: {
                mFeatureDetector =  SiftFeatureDetector::create();      //or makePtr<SiftFeatureDetector>();       //it was (on opencv 2.x): = new SiftFeatureDetector();
                mDescriptorExtractor = SiftDescriptorExtractor::create();  //or makePtr<SiftDescriptorExtractor>()    //it was (on opencv 2.x): = new SiftDescriptorExtractor();
                Log(log_Error, "model.cpp", "initialize", "         Done.");

                break;
            }
            case feature_SURF:
            case feature_ORB:
            case feature_LBP:
            case feature_FAST:
            case feature_BRIEF:
            case feature_START:
            case feature_MSER:
            case feature_GFTT:
            case feature_HARRIS:
            case feature_DENSE:
            case feature_BLOB:
                Log(log_Error, "model.cpp", "initialize", "         ERROR: FEATURE NOT IMPLEMENTED.");
                return false;
        }

        Log(log_Error, "model.cpp", "initialize", "      Initializing matcher module: '" + getMatcherName() + "'...");
        switch (mMatcherType)
        {
            case matcher_FLANN: {
                //TODO: Isso aqui mudou! Alterar
                mDescriptorMatcher = new FlannBasedMatcher();
                Log(log_Error, "model.cpp", "initialize", "         Done.");
                break;
            }
            case matcher_K_MEANS_CLUSTERING:
            case matcher_BRUTE_FORCE:
                //mDescriptorMatcher matcher = new BFMatcher::create("BruteForce");
                Log(log_Error, "model.cpp", "initialize", "         ERROR: MATCHER NOT IMPLEMENTED.");
                return false;
        }

        Log(log_Error, "model.cpp", "initialize", "      Initializing classifier module: '" + getClassifierName() + "'...");
        switch (mClassifierType)
        {
            case model_BAG_OF_FEATURES:{
                mTrainer = new BOWKMeansTrainer(mDictionarySize,  TermCriteria(CV_TERMCRIT_ITER, 10, 0.001), 1, KMEANS_PP_CENTERS);

                Log(log_Error, "model.cpp", "initialize", "         Creating BOW feature extractor...");
                mBOWDescriptorExtractor = new BOWImgDescriptorExtractor(mDescriptorExtractor, mDescriptorMatcher);
                Log(log_Error, "model.cpp", "initialize", "            Done.");

                Log(log_Error, "model.cpp", "initialize", "         Done.");
                break;
            }
            case model_PROJECTION:{
                Log(log_Error, "model.cpp", "initialize", "         ERROR: CLASSIFIER NOT IMPLEMENTED.");
                return false;
            }

        }

        startTask = getTick();
        Log(log_Error, "model.cpp", "initialize", "      Creating SVM (Support Vector Machine)...");
        mSupportVectorMachine = SVM::create();
        Log(log_Error, "model.cpp", "initialize", "         Done.");

        Log(log_Error, "model.cpp", "initialize", "      Configuring SVM params...");

        mSupportVectorMachine->setKernel(SVM::RBF);
        Log(log_Error, "model.cpp", "initialize", "         Kernel: RBF");

        mSupportVectorMachine->setType(SVM::C_SVC);
        Log(log_Error, "model.cpp", "initialize", "         Type: SVC");

        mSupportVectorMachine->setGamma(0.50625000000000009);
        Log(log_Error, "model.cpp", "initialize", "         Gamma: 0.50625000000000009");

        mSupportVectorMachine->setC(312.50000000000000);
        Log(log_Error, "model.cpp", "initialize", "         C: 312.5");

        mSupportVectorMachine->setTermCriteria(TermCriteria(CV_TERMCRIT_ITER, 100, 0.000001));
        Log(log_Error, "model.cpp", "initialize", "         Done.");

        Log(log_Error, "model.cpp", "initialize", "      Done. Initialization took %s seconds.",  getDiffString(startTask).c_str());
        return true;

    }catch(const std::exception& e){
        Log(log_Error, "model.cpp", "initialize",  "      Error creating model: %s", e.what()) ;
    }

}

bool Model::loadSamples(string sampleFolder) {

    int64 startTask = getTick();

    try{
        //Is the input folder actually an existing folder?
        if(isFolder(sampleFolder)) {

            //Load all files recursivelly
            vector<string> files = listFiles(sampleFolder);

            Log(log_Debug, "model.cpp", "loadSamples", "      Loading samples...");

            mClasses.clear();

            //Iterate all files
            for (int i = 0; i < files.size(); i++) {

                //creates a class label based on the folder where the file is located
                string className = replace(getFolderName(files[i]), sampleFolder, "");

                //Searches for this class
                int k;
                for (k = 0; k < mClasses.size(); k++) {
                    if (mClasses[k].getLabel() == className) {
                        break;
                    }
                }

                //have we seen this class before?
                if (k == mClasses.size()) {
                    //Nope. Adds it to the classes vector
                    Class c(className);
                    mClasses.push_back(c);
                }

                //creates the sample
                Sample s;
                s.load(files[i], className, false);

                //adds this sample to the class
                mClasses[k].samples.push_back(s);

            }

            Log(log_Debug, "model.cpp", "loadSamples", "         %i samples were loaded.", files.size());
            Log(log_Debug, "model.cpp", "loadSamples", "         %i classes were found:", mClasses.size());

            for (int i = 0; i < mClasses.size(); i++) {

                mClasses[i].calculateAverageSampleHeight();
                mClasses[i].calculateAverageSampleWidth();

                Log(log_Debug, "model.cpp", "loadSamples", "            %i) '%s' (Average size is W:%i x H:%i)", (i + 1), mClasses[i].getLabel().c_str(), mClasses[i].getAverageSampleWidth(), mClasses[i].getAverageSampleHeight());
                for (int k = 0; k < mClasses[i].samples.size(); k++) {
                    mAverageSampleWidth = mAverageSampleWidth + mClasses[i].samples[k].originalMat.cols;
                    mAverageSampleHeight = mAverageSampleHeight + mClasses[i].samples[k].originalMat.rows;
                }

            }
            if (files.size() > 0) {
                mAverageSampleWidth = mAverageSampleWidth / files.size();
                mAverageSampleHeight = mAverageSampleHeight / files.size();
                Log(log_Debug, "model.cpp", "loadSamples", "         Average sample size is W:%i x H:%i.", mAverageSampleWidth, mAverageSampleHeight);
            }

            Log(log_Debug, "model.cpp", "loadSamples", "      Done. Loading samples took %s seconds.", getDiffString(startTask).c_str());
            return true;

        }else
            Log(log_Error, "model.cpp", "loadSamples",  "   Sample folder does not exit!");

    }catch(const std::exception& e){
        Log(log_Error, "model.cpp", "loadSamples",  "      Error loading samples: %s", e.what()) ;
    }

    return false;

}

bool Model::preProcessSamples() {

    int64 startTask = getTick();

    long sampleCount = 0;
    long validSampleCount = 0;

    try{
        Log(log_Debug, "model.cpp", "preProcessSamples", "      Pre-processing samples...");

        for (int i = 0; i < mClasses.size(); i++) {

            for (int k = 0; k < mClasses[i].samples.size(); k++) {

                sampleCount++;

                Log(log_Debug, "model.cpp", "preProcessSamples", "         Pre-processing sample %05d...", sampleCount);

                if(mClasses[i].samples[k].preProcess(mMinDimension, mMaxDimension, mClasses[i].getAverageSampleWidth(), mClasses[i].getAverageSampleHeight(), mBinarizationType))
                    validSampleCount++;

            }
        }

        Log(log_Error, "model.cpp", "preProcessSamples", "      Done. %i of %i samples were valid. Pre-processing took %s seconds.", validSampleCount, sampleCount, getDiffString(startTask).c_str());
        return (validSampleCount > 0);

    }catch(const std::exception& e){
        Log(log_Error, "model.cpp", "preProcessSamples",  "      Error creating model: %s", e.what()) ;
    }

    return false;
}

bool Model::createDictionary() {

    int64 startTask = getTick();
    int64 startSubtask;

    long sampleCount = 0;
    long validSampleCount = 0;

    try{
        startSubtask = getTick();
        Log(log_Debug, "model.cpp", "createDictionary", "      Creating new dictionary from valid samples...");

        for (int i = 0; i < mClasses.size(); i++) {

            for (int k = 0; k < mClasses[i].samples.size(); k++) {

                Mat m = mClasses[i].samples[k].binaryMat;
                if (isMatValid(m)) {

                    sampleCount++;
                    Log(log_Debug, "model.cpp", "createDictionary", "         Processing sample %05d...", sampleCount);

                    Log(log_Detail, "model.cpp", "createDictionary", "            Extracting features...");
                    mFeatureDetector->detect(m, mClasses[i].samples[k].features);
                    if (mClasses[i].samples[k].features.size() > 0) {
                        Log(log_Detail, "model.cpp", "createDictionary","               Computing descriptors from the %i extracted features...", mClasses[i].samples[k].features.size());
                        mDescriptorExtractor->compute(m, mClasses[i].samples[k].features,  mClasses[i].samples[k].dic_descriptors);
                        if (!mClasses[i].samples[k].dic_descriptors.empty()) {
                            validSampleCount++;
                            Log(log_Detail, "model.cpp", "createDictionary","                  Adding descriptors to Trainer...");
                            mTrainer->add(mClasses[i].samples[k].dic_descriptors);
                            Log(log_Detail, "model.cpp", "createDictionary","                     Done. Trainer has %i descriptors.", mTrainer->descriptorsCount());
                        } else
                            Log(log_Error, "model.cpp", "createDictionary","            Ignoring sample because no descriptors were computed.");
                    } else
                        Log(log_Error, "model.cpp", "createDictionary","            Ignoring sample because no features were extracted.");
                }
            }
        }
        Log(log_Debug, "model.cpp", "createDictionary", "         Done. Processing samples took %s seconds.", getDiffString(startSubtask).c_str());

        //Did processing the samples find anything usefull?
        if (mTrainer->descriptorsCount() > 0){

            startSubtask = getTick();
            Log(log_Debug, "model.cpp", "createDictionary", "      Clustering the %i descriptors from the %i valid samples (choosing centroids as words)...", mTrainer->descriptorsCount(), validSampleCount);
            mDictionary = mTrainer->cluster();
            Log(log_Debug, "model.cpp", "createDictionary", "         Done. Clustering took %s seconds.", getDiffString(startSubtask).c_str());

            Log(log_Debug, "model.cpp", "createDictionary", "      Done. Created the dictionary (%i words, %i items) in %s seconds.", mDictionary.rows, mDictionary.cols, getDiffString(startTask).c_str());
            return (!mDictionary.empty());
        }

        Log(log_Error, "model.cpp", "createDictionary", "       Create dictionary failed!");

    }catch(const std::exception& e){
        Log(log_Error, "model.cpp", "createDictionary",  "      Error creating dictionary: %s", e.what()) ;
    }

    return false;

}

bool Model::prepareTrainingSet() {

    int64 startTask = getTick();
    int64 startSubtask;

    long sampleCount = 0;
    long validSampleCount = 0;

    try{
        Log(log_Debug, "model.cpp", "prepareTrainingSet", "   Preparing training set...");

        Log(log_Error, "model.cpp", "prepareTrainingSet", "      Setting vocabulary...");
        mBOWDescriptorExtractor->setVocabulary(mDictionary);
        Log(log_Error, "model.cpp", "prepareTrainingSet", "         Done.");

        Log(log_Error, "model.cpp", "prepareTrainingSet", "      Preparing samples...");
        startSubtask = getTick();
        for (int i = 0; i < mClasses.size(); i++) {

            for (int k = 0; k < mClasses[i].samples.size(); k++) {

                //Check if this sample has the features (saved on createDictionary)
                if(mClasses[i].samples[k].features.size() > 0){

                    sampleCount++;
                    Log(log_Error, "model.cpp", "prepareTrainingSet", "         Preparing sample %05d...", sampleCount);

                    Mat m = mClasses[i].samples[k].binaryMat;
                    Log(log_Detail, "model.cpp", "prepareTrainingSet", "         Computing descriptors from the %i features...", mClasses[i].samples[k].features.size());
                    mBOWDescriptorExtractor->compute(m, mClasses[i].samples[k].features, mClasses[i].samples[k].bow_descriptors);

                    //TODO: GOTCHA 4: Is the bow_descriptos the same as the dic_descriptors?
                    if (!mClasses[i].samples[k].bow_descriptors.empty()) {
                        validSampleCount++;
                        Log(log_Detail, "model.cpp", "prepareTrainingSet", "            Adding descriptors and label to the training data...");
                        mTrainingData.push_back(mClasses[i].samples[k].bow_descriptors);
                        mTrainingLabel.push_back(i);
                        Log(log_Detail, "model.cpp", "prepareTrainingSet", "               Done.");
                    }
                }
            }
        }
        Log(log_Error, "model.cpp", "prepareTrainingSet", "         Done. Preparing samples took %s seconds.", getDiffString(startTask).c_str());

        Log(log_Debug, "model.cpp", "prepareTrainingSet", "      Done. Preparing training set took %s seconds.", getDiffString(startSubtask).c_str());
        return (!mTrainingData.empty() && !mTrainingLabel.empty());

    }catch(const std::exception& e){
        Log(log_Error, "model.cpp", "prepareTrainingSet",  "      Error preparing training set: %s", e.what()) ;
    }

    return false;

}

string Model::getClassifierName(){
    switch (mClassifierType){
        case model_PROJECTION:	    return "PROJECTION";
        case model_BAG_OF_FEATURES:	return "BAG OF FEATURES";
        default:				    return "UNKNOWN";
    }
}

string Model::getFeatureName(){
    switch (mFeatureType){
        case feature_SIFT:	return "SIFT (Scale Invariant Feature Transform)";
        case feature_SURF:	return "SURF (Speeded Up Robust Features)";
        case feature_LBP:	return "LBP (Local Binary Patters)";
        case feature_FAST:	return "FAST (Feature From Accelerated Segment Tests)";
        case feature_BRIEF:	return "BRIEF (Binary Robust Independent Elementary Features)";
        case feature_ORB:	return "ORB (Oriented Fast & Rotated BRIEF)";
        default:			return "UNKNOWN";
    }
}

string Model::getMatcherName(){
    switch (mMatcherType){
        case matcher_BRUTE_FORCE:		    return "BRUTE FORCE";
        case matcher_FLANN:                 return "FLANN (Fast Library for Approximating Nearest Neighbors)";
        case matcher_K_MEANS_CLUSTERING:	return "K MEANS CLUSTERING";
        default:				            return "UNKNOWN";
    }
}

string Model::getBinarizationName(){
    switch (mBinarizationType){
        case binarization_TRESHOLD:	  return "TRESHOLD (Byte values bigger than thresold are white, lowe is black. Threshold is 127)";
        case binarization_MEAN:	      return "MEAN (treshold is the mean of neighbourhood area)";
        case binarization_GAUSSIAN:	  return "GAUSSIAN (threshold is weighted sum of neighbourhood values where weights are a gaussian window";
        case binarization_NIBLACK:	  return "NIBLACK (Wayne Niblack)";
        case binarization_SAUVOLA:    return "SAUVOLA (Jaakko J. Sauvola)";
        case binarization_WOLFJOLION: return "WOLFJOLION (Christian Wolf, Jean-Michel Jolion and Francoise Chassaing)";
        case binarization_BRADLEY:	  return "BRADLEY (Derek Bradley)";
        case binarization_CLAHE:	  return "CLAHE (Contrast Limited Adaptive Histogram Equalization)";
        default:				      return "UNKNOWN";
    }
}

void Model::setClassifierType(enumClassifier type) {
    mClassifierType = type;
    Log(log_Debug, "model.cpp", "setClassifierType", "Classifier was set to '%s'.", getClassifierName().c_str());
}

void Model::setFeatureType(enumFeature type) {
    mFeatureType = type;
    Log(log_Debug, "model.cpp", "setFeatureType", "Feature was set to '%s'.", getFeatureName().c_str());
}

void Model::setMatcherType(enumMatcher type) {
    mMatcherType = type;
    Log(log_Debug, "model.cpp", "setMatcherType", "Matcher was set to '%s'.", getMatcherName().c_str());
}

void Model::setBinarizationType(enumBinarization type) {
    mBinarizationType = type;
    Log(log_Debug, "model.cpp", "setBinarizationType", "Binarization was set to '%s'.", getBinarizationName().c_str());
}

void Model::setFilename(string filename) {
    mFilename = filename;
    Log(log_Debug, "model.cpp", "setFilename", "Filename was set to '%s'.", mFilename.c_str());
}

string Model::getFilename() {
    return mFilename;
}

bool Model::classify(string path){

    int64 startTask = getTick();
    int64 startSubtask;

    vector<KeyPoint> features;
    Mat m;
    Mat	descriptor;
    string sRet;

    Log(log_Debug, "model.cpp", "classify", "   Classifing path '%s'...", path.c_str());

    startSubtask = getTick();
    Log(log_Debug, "model.cpp", "classify", "      Extracting keypoints...");
    mFeatureDetector->detect(m, features);
    Log(log_Debug, "model.cpp", "classify", "         Done in %s seconds.",  getDiffString(startSubtask).c_str());

    startSubtask = getTick();
    Log(log_Debug, "model.cpp", "classify", "      Setting vocabulary...");
    mBOWDescriptorExtractor->setVocabulary(mDictionary);
    Log(log_Debug, "model.cpp", "classify", "         Done in %s seconds.",  getDiffString(startSubtask).c_str());

    startSubtask = getTick();
    Log(log_Debug, "model.cpp", "classify", "      Extracting descriptors from keypoints...");
    mBOWDescriptorExtractor->compute(m, features, descriptor);
    Log(log_Debug, "model.cpp", "classify", "         Done in %s seconds.",  getDiffString(startSubtask).c_str());

    startSubtask = getTick();
    Log(log_Debug, "model.cpp", "classify", "      Predicting from file's descriptors...");
    float response = mSupportVectorMachine->predict(descriptor);
    Log(log_Debug, "model.cpp", "classify", "         Done in %s seconds.",  getDiffString(startSubtask).c_str());

    if (response >= 0){
       //Log(log_Debug, "model.cpp", "classify", "      Predict returned " + sampleClass[response].label + " (" + to_string(response) + ")!");
       //sRet = sampleClass[response];
    }
    else{
       Log(log_Debug, "model.cpp", "classify", "      Predict returned UNKNOWN!");
       sRet = "UNKNOWN";
    }

    Log(log_Debug, "model.cpp", "classify", "         Done in %s seconds.",  getDiffString(startTask).c_str());

}
