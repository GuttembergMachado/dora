//
// Guttemberg Machado on 24/07/17.
//
//      -----------------------+------------------------------+
//      |      TRAINING:       |        CLASSIFYING           |
//      +----------------------+------------------------------+
//      | List files           | List file or folder          |
//      +----------------------+------------------------------+
//      | Iterate Files        |                              |
//      |    Extract samples   |                              |
//      +----------------------+------------------------------+
//      | Iterate samples      | Load dictionary from file    |
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

	try{
        Log(log_Error, "model.cpp", "create", "   Creating model...");

        //Is the input folder actually an existing folder?
        if(isFolder(sampleFolder)) {

            //Load all files recursivelly
            vector<string> files = listFiles(sampleFolder);
            samples.clear();

            Log(log_Debug, "model.cpp", "create", "      Loading samples...");
            startSubtask = getTick();
            for (int i = 0; i < files.size(); i++) {

                Sample s;

                //Loads the sample
                if (s.load(files[i])) {

                    //Check if sample is too small
                    if (s.originalMat.cols > minDimension && s.originalMat.rows > minDimension) {

                        //Check if sample needs resizing
                        if (s.originalMat.cols < maxDimension && s.originalMat.rows < maxDimension) {

                            //Uses the folder as the label
                            s.label = replace(getFolderName(s.filename), sampleFolder, "");

                            //Uses this sample
                            samples.push_back(s);

                            Log(log_Debug, "model.cpp", "create", "         File %05d was LOADED as Sample %i ('%s')...", (i + 1), samples.size(), files[i].c_str());

                            //Update class statistics
                            updateClass(s);

                            s.dump();

                        }else
                            Log(log_Error, "model.cpp", "create", "          File %05d was IGNORED because it is LARGER than %i pixels ('%s')!", (i + 1), maxDimension, files[i].c_str());
                    }else
                        Log(log_Error, "model.cpp", "create", "         File %05d was IGNORED because it is SMALLER than %i pixels ('%s')!", (i + 1), minDimension, files[i].c_str());
                }
            }
            Log(log_Debug, "model.cpp", "create", "         Done. Loaded %i samples from %i files in %s seconds.",(samples.size()), files.size(), getDifString(startSubtask).c_str());


            Log(log_Debug, "model.cpp", "create", "      Checking all %i classes:", sampleClass.size() );
            for (int k = 0; k < sampleClass.size(); k++) {
                Log(log_Debug, "model.cpp", "create", "         Class %i ('%s') has %i samples with an average size of %iW X %iH pixels;", (k + 1), sampleClass[k].label.c_str(), sampleClass[k].count, sampleClass[k].width / sampleClass[k].count,  sampleClass[k].height / sampleClass[k].count );
            }
            Log(log_Debug, "model.cpp", "create", "         Done. All classes were checked.", sampleClass.size() );

            //Creates the dictionary
            if(preProcessSamples()) {

                //Creates the dictionary
                if(createDictionary()){

                    //Creates the training set (data + labels)
                    if(prepareTrainingSet()) {

                        Log(log_Error, "model.cpp", "create", "   Training the SVM...");
                        startSubtask = getTick();
                        bool res = svm->train(trainingData, ROW_SAMPLE, trainingLabel);  //(ROW_SAMPLE: each training sample is a row of samples; COL_SAMPLE :each training sample occupies a column of samples)
                        Log(log_Error, "model.cpp", "create", "   Done training model in %s seconds.", getDifString(startSubtask).c_str());

                        Log(log_Error, "model.cpp", "create", "   Done. Creating model %s %s seconds.", (res ? "took" : "failed after"), getDifString(startTask).c_str());
                        return res;

                    }else
                        Log(log_Debug, "model.cpp", "create", "   Failed to prepare training set!");

                }else
                    Log(log_Debug, "model.cpp", "create", "   Failed to create dictionary!");
            }else
                Log(log_Debug, "model.cpp", "create", "   Failed to pre-process samples!");
        }else
            Log(log_Error, "model.cpp", "create",  "   Sample folder does not exit!");

    }catch(const std::exception& e){
        Log(log_Error, "model.cpp", "create",  "   Error creating model: %s", e.what()) ;
    }

    return false;

}

bool Model::load(){

    try{

        if (isFile(filename)){
            Log(log_Debug, "model.cpp", "load", "   Loading model file...");

            //Reads the model
            //svm.load(filename.c_str());
            string trainingSetFile = filename + "_trainingSetFile.xml";

            if (isFile(trainingSetFile)){

                FileStorage fs(trainingSetFile, FileStorage::READ);
                fs["dictionary"] >> dictionary;
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

	Log(log_Debug, "model.cpp", "save", "   Saving model file...");

	try{

        Log(log_Debug, "model.cpp", "save", "      Saving dictionary on file '" + filename + "'...");
        startTask = getTick();

        //Saves
        FileStorage fs(filename, FileStorage::WRITE);
        fs << "dictionary" << dictionary;
        fs << "trainingData" << trainingData;

        fs.release();

        //svm.save(filename.c_str());

        Log(log_Debug, "model.cpp", "save", "         Done saving dictionary file in %s seconds.",  getDifString(startTask).c_str());

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

        //TODO: 1) ALL THE 'initialize functions' from OPENCV changed from version 2.x to 3.x.  CHECK THIS CAREFULLY
        //TODO: 2) Check what is the optimized size of a dictionary (based on samples, number of labels, etc)

        dictionarySize = 1500;
        Log(log_Error, "model.cpp", "initialize", "      Dictionary size is %i.", dictionarySize);

        trainingData  = Mat(0, dictionarySize, CV_32S);
        Log(log_Error, "model.cpp", "initialize", "      TrainingData mat is type '%s'. ",  getMatType(trainingData).c_str());

        trainingLabel = Mat(0, 1, CV_32S);
        Log(log_Error, "model.cpp", "initialize", "      TrainingLabel mat is type '%s'.", getMatType(trainingLabel).c_str());

        Log(log_Error, "model.cpp", "initialize", "      Initializing detector module: '" + getFeatureName() + "'...");
        switch (featureType)
        {
            case feature_SIFT: {
                detector =  SiftFeatureDetector::create();      //or makePtr<SiftFeatureDetector>();       //it was (on opencv 2.x): = new SiftFeatureDetector();
                extractor = SiftDescriptorExtractor::create();  //or makePtr<SiftDescriptorExtractor>()    //it was (on opencv 2.x): = new SiftDescriptorExtractor();
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
                Log(log_Error, "model.cpp", "initialize", "         ERROR: FEATURE '" + getFeatureName() + "' NOT IMPLEMENTED.");
                return false;
        }

        Log(log_Error, "model.cpp", "initialize", "      Initializing matcher module: '" + getMatcherName() + "'...");
        switch (matcherModel)
        {
            case matcher_FLANN: {
                //TODO: Isso aqui mudou! Alterar
                matcher = new FlannBasedMatcher();
                Log(log_Error, "model.cpp", "initialize", "         Done.");
                break;
            }
            case matcher_K_MEANS_CLUSTERING:
            case matcher_BRUTE_FORCE:
                //matcher = new BFMatcher::create("BruteForce");
                Log(log_Error, "model.cpp", "initialize", "         ERROR: MATCHER '" + getMatcherName() + "' NOT IMPLEMENTED.");
                return false;
        }

        Log(log_Error, "model.cpp", "initialize", "      Initializing trainer module: '" + getModelName() + "'...");
        switch (classificationModel)
        {
            case model_BAG_OF_FEATURES:{
                trainer = new BOWKMeansTrainer(dictionarySize,  TermCriteria(CV_TERMCRIT_ITER, 10, 0.001), 1, KMEANS_PP_CENTERS);
                Log(log_Error, "model.cpp", "initialize", "         Done.");
                break;
            }
            case model_PROJECTION:{
                Log(log_Error, "model.cpp", "initialize", "         ERROR: TRAINER '" + getModelName() + "' NOT IMPLEMENTED.");
                return false;
            }

        }

        startTask = getTick();
        Log(log_Error, "model.cpp", "initialize", "      Creating SVM (Support Vector Machine)...");
        svm = SVM::create();
        Log(log_Error, "model.cpp", "initialize", "         Done.");

        Log(log_Error, "model.cpp", "initialize", "      Configuring SVM params...");

        svm->setKernel(SVM::RBF);
        Log(log_Error, "model.cpp", "initialize", "         Kernel: RBF");

        svm->setType(SVM::C_SVC);
        Log(log_Error, "model.cpp", "initialize", "         Type: SVC");

        svm->setGamma(0.50625000000000009);
        Log(log_Error, "model.cpp", "initialize", "         Gamma: 0.50625000000000009");

        svm->setC(312.50000000000000);
        Log(log_Error, "model.cpp", "initialize", "         C: 312.5");

        svm->setTermCriteria(TermCriteria(CV_TERMCRIT_ITER, 100, 0.000001));
        Log(log_Error, "model.cpp", "initialize", "         Done.");

        Log(log_Error, "model.cpp", "initialize", "      Done. Initialization took %s seconds.",  getDifString(startTask).c_str());
        return true;

    }catch(const std::exception& e){
        Log(log_Error, "model.cpp", "initialize",  "      Error creating model: %s", e.what()) ;
    }

}

bool Model::createDictionary() {

    int64 startTask = getTick();
    int64 startSubtask;

    vector<KeyPoint> keypoints;
    Mat descriptors;

    try{
        Log(log_Debug, "model.cpp", "createDictionary", "      Creating a new dictionary based on the %i samples found...", samples.size());
        startSubtask= getTick();

        for (int i = 0; i < samples.size(); i++) {

            Log(log_Debug, "model.cpp", "createDictionary", "         Processing sample %05d...", (i+1));

            Log(log_Detail, "model.cpp", "createDictionary", "            Extracting features...");
            detector->detect(samples[i].workMat, keypoints);
            if(keypoints.size() > 0){
                Log(log_Detail, "model.cpp", "createDictionary", "            Computing descriptors from features...");
                extractor->compute(samples[i].workMat, keypoints, descriptors);
                if (descriptors.cols > 0){
                    Log(log_Detail, "model.cpp", "createDictionary", "            Saving descriptors...");
                    trainer->add(descriptors);
                }else
                    Log(log_Error, "model.cpp", "prepareTrainingSet", "               Failed: No descriptors were found on sample %i ('%s'), even though %i keypoints were found...", (i + 1), keypoints.size(), samples[i].filename.c_str() );
            }else
                Log(log_Error, "model.cpp", "prepareTrainingSet", "               Failed: No features (key points) were found on sample %i ('%s')...", (i + 1), samples[i].filename.c_str() );

        }
        Log(log_Debug, "model.cpp", "createDictionary", "         Done. Processing all %i samples took %s seconds.", samples.size(), getDifString(startSubtask).c_str());

        //Did processing the samples find anything usefull?
        if (trainer->descriptorsCount() > 0){

            Log(log_Debug, "model.cpp", "createDictionary", "      Clustering (choosing centroids as words) features out of all %i descriptors found...", trainer->descriptorsCount());
            startSubtask = getTick();

            //Cluster (use the centroid of each cluster as the words of the dictionary)
            dictionary = trainer->cluster();

            Log(log_Debug, "model.cpp", "createDictionary", "         Done. Clustering took %s seconds.", getDifString(startSubtask).c_str());

            if(dictionary.dims > 0){
                Log(log_Debug, "model.cpp", "createDictionary", "      Done. Creating dictionary took %s seconds.", getDifString(startTask).c_str());
                return true;
            }else
                Log(log_Debug, "model.cpp", "createDictionary", "      ERROR: Cluster failed to create a dictionary!");
        }else
            Log(log_Error, "model.cpp", "createDictionary", "       ERROR: Failed to cluster because no descriptors were found!");

    }catch(const std::exception& e){
        Log(log_Error, "model.cpp", "createDictionary",  "      Error creating dictionary: %s", e.what()) ;
    }

    return false;

}

bool Model::prepareTrainingSet() {

    int64 startTask = getTick();
    int64 startSubtask;

    Log(log_Debug, "model.cpp", "prepareTrainingSet", "   Preparing training set (using all %i samples)...", samples.size());

    Log(log_Error, "model.cpp", "prepareTrainingSet", "      Creating bag of words extractor...");
    BOWImgDescriptorExtractor bow(extractor, matcher);
    Log(log_Error, "model.cpp", "prepareTrainingSet", "         Done.");

    Log(log_Error, "model.cpp", "prepareTrainingSet", "      Setting vocabulary...");
    bow.setVocabulary(dictionary);
    Log(log_Error, "model.cpp", "prepareTrainingSet", "         Done.");

    Log(log_Error, "model.cpp", "prepareTrainingSet", "      Preparing data (trainingData mat is '%s')...", getMatType(trainingData).c_str());
    startSubtask = getTick();
    for (int i = 0; i < samples.size(); i++) {

        Log(log_Debug, "model.cpp", "prepareTrainingSet", "         Preparing sample %05d...", (i + 1));

        vector<KeyPoint> keypoints;
        Mat descriptors;

        Log(log_Detail, "model.cpp", "prepareTrainingSet", "            Extracting features (key points)...");
        detector->detect(samples[i].binaryMat, keypoints);
        if(keypoints.size() > 0){
            Log(log_Detail, "model.cpp", "prepareTrainingSet",  "            Computing descriptors from features (key points)...");
            bow.compute(samples[i].binaryMat, keypoints, descriptors);
            if (descriptors.cols > 0){
                Log(log_Detail, "model.cpp", "prepareTrainingSet", "            Adding %i descriptors to trainingData mat (that has '%i' items so far)...", descriptors.cols, trainingData.rows);
                trainingData.push_back(descriptors);
            }else
                Log(log_Error, "model.cpp", "prepareTrainingSet", "            Failed: No descriptors were found on sample %i ('%s'), eventhough %i keypoints were found...", (i + 1), keypoints.size(), samples[i].filename.c_str() );
        }else
            Log(log_Error, "model.cpp", "prepareTrainingSet", "            Failed: No features (key points) were found on sample %i ('%s')...", (i + 1), samples[i].filename.c_str() );

    }
    Log(log_Error, "model.cpp", "prepareTrainingSet", "         Done. Preparing data from all %i samples took %s seconds (trainingData mat is '%s')", samples.size() , getDifString(startSubtask).c_str(), getMatType(trainingData).c_str() );

    Log(log_Debug, "model.cpp", "prepareTrainingSet", "      Preparing label mat (trainingLabel mat is '%s')...", getMatType(trainingLabel).c_str());
    startSubtask = getTick();
    for (int i = 0; i < samples.size(); i++) {

        for (int k = 0; k < sampleClass.size(); k++){
            if(sampleClass[k].label == samples[i].label){
                Log(log_Debug, "model.cpp", "prepareTrainingSet",  "         Sample %05d is from class '%s'. Label is %i.", (i + 1), sampleClass[k].label.c_str(), k );
                trainingLabel.push_back(k);
                break;
            }
        }
    }
    Log(log_Error, "model.cpp", "prepareTrainingSet", "         Done.");

    if(isMatValid(trainingData)){
        if(trainingData.rows == trainingLabel.rows) {
            Log(log_Debug, "model.cpp", "prepareTrainingSet", "      Done. Preparing training set took %s seconds.", getDifString(startTask).c_str());
            return true;
        }else{
            Log(log_Error, "model.cpp", "prepareTrainingSet", "      Failed to prepare training set. TrainingData has %i cols and TrainingLabels has %i!", trainingData.rows, trainingLabel.rows );
        }

    }else{
        Log(log_Error, "model.cpp", "prepareTrainingSet", "      Failed to create trainingData after %s seconds.", getDifString(startTask).c_str());
    }

    return false;

}

bool Model::preProcessSamples() {

    int64 startTask = getTick();
    int64 startSubtask;
    int i;
    int classWidth = averageDimension;
    int classHeight = averageDimension;

    try{
        Log(log_Debug, "model.cpp", "preProcessSamples", "      Pre-processing all %i samples...", samples.size());

        for (i = 0; i < samples.size(); i++) {

            Log(log_Debug, "model.cpp", "preProcessSamples", "         Pre-processing sample %05d...", (i+1));

            //for (int k = 0; k < sampleClass.size(); k++) {
            //    if((sampleClass[k].label == samples[i].label) ){
            //        classWidth = sampleClass[i].width / sampleClass[i].count;
            //        classHeight = sampleClass[i].height / sampleClass[i].count;
            //        break;
            //    }
            //}
            //Log(log_Debug, "model.cpp", "preProcessSamples", "         Sample %i will be resized from W:%i x H:%i to W:%i x H:%i...", (i + 1) , samples[i].originalMat.cols, samples[i].originalMat.rows, classWidth, classHeight);

            //Creates the working Mat keeping the recomended class size
            if (!samples[i].createWorkMat(classWidth, classHeight))
                break;

            //Creates grayscale mat of the sample
            if (!samples[i].createGrayscaleMat())
                break;

            //Creates monochromatic mat of the grayscale sample
            if (!samples[i].createBinaryMat(binarizationMethod))
                break;

            //Creates XYCut images
            if (!samples[i].createXYCutMat())
                break;

            if (saveIntermediateFiles)
                samples[i].saveIntermediate(folder, false, true, false, false, false);

        }
        Log(log_Debug, "model.cpp", "preProcessSamples", "         Done. Pre-processing all %i samples took %s seconds.", samples.size(), getDifString(startTask).c_str());

        return (i >= samples.size());

    }catch(const std::exception& e){
        Log(log_Error, "model.cpp", "preProcessSamples",  "      Error creating model: %s", e.what()) ;
    }

    return false;
}

void Model::updateClass(Sample s){

    int i;

    Log(log_Detail, "model.cpp", "updateClass", "         Updating class statistics...");
    for (i = 0; i < sampleClass.size(); i++) {
        if(sampleClass[i].label == s.label)
           break;
    }

    if (i >= sampleClass.size()){
        Log(log_Detail, "model.cpp", "updateClass", "         This is a new class ('%s')...", s.label.c_str());
        Class c;
        c.label = s.label;
        c.count = 0;
        c.width = 0;
        c.height = 0;
        sampleClass.push_back(c);
    }

    sampleClass[i].count++;
    sampleClass[i].width = sampleClass[i].width + s.originalMat.cols;
    sampleClass[i].height = sampleClass[i].height + s.originalMat.rows;

    Log(log_Detail, "model.cpp", "updateClass", "            Done. Class statistics were updated.");

}

bool Model::classify(string path){

    int64 startTask = getTick();
    int64 startSubtask;

    vector<KeyPoint> keypoints;
    Mat m;
    Mat	descriptor;
    string sRet;

    Log(log_Debug, "model.cpp", "classify", "   Classifing path '%s'...", path.c_str());

    startSubtask = getTick();
    Log(log_Debug, "model.cpp", "classify", "      Extracting keypoints...");
    detector->detect(m, keypoints);
    Log(log_Debug, "model.cpp", "classify", "         Done in %s seconds.",  getDifString(startSubtask).c_str());

    startSubtask = getTick();
    Log(log_Debug, "model.cpp", "classify", "      Creating BOW exctractor...");
    BOWImgDescriptorExtractor bow(extractor, matcher);
    Log(log_Debug, "model.cpp", "classify", "         Done in %s seconds.",  getDifString(startSubtask).c_str());

    startSubtask = getTick();
    Log(log_Debug, "model.cpp", "classify", "      Setting vocabulary...");
    bow.setVocabulary(dictionary);
    Log(log_Debug, "model.cpp", "classify", "         Done in %s seconds.",  getDifString(startSubtask).c_str());

    startSubtask = getTick();
    Log(log_Debug, "model.cpp", "classify", "      Extracting descriptors from keypoints...");
    bow.compute(m, keypoints, descriptor);
    Log(log_Debug, "model.cpp", "classify", "         Done in %s seconds.",  getDifString(startSubtask).c_str());

    startSubtask = getTick();
    Log(log_Debug, "model.cpp", "classify", "      Predicting from file's descriptors...");
    float response = svm->predict(descriptor);
    Log(log_Debug, "model.cpp", "classify", "         Done in %s seconds.",  getDifString(startSubtask).c_str());

    if (response >= 0){
       //Log(log_Debug, "model.cpp", "classify", "      Predict returned " + sampleClass[response].label + " (" + to_string(response) + ")!");
       //sRet = sampleClass[response];
    }
    else{
       Log(log_Debug, "model.cpp", "classify", "      Predict returned UNKNOWN!");
       sRet = "UNKNOWN";
    }

    Log(log_Debug, "model.cpp", "classify", "         Done in %s seconds.",  getDifString(startTask).c_str());

}

string Model::getMatType(Mat m){
    switch (m.type()){
        case CV_8U:   return "CV_8U";
        case CV_8S:   return "CV_8S";
        case CV_16U:  return "CV_16U";
        case CV_16S:  return "CV_16S";
        case CV_32S:  return "CV_32S";
        case CV_32F:  return "CV_32F";
        case CV_64F:  return "CV_64F";
        default:      return "UNKNONW";
    }
}

string Model::getModelName(){
    switch (classificationModel)
    {
        case model_PROJECTION:	    return "PROJECTION";
        case model_BAG_OF_FEATURES:	return "BAG OF FEATURES";
        default:				    return "UNKNOWN";
    }
};

string Model::getFeatureName(){
    switch (featureType)
    {
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
    switch (matcherModel)
    {
        case matcher_BRUTE_FORCE:		    return "BRUTE FORCE";
        case matcher_FLANN:                 return "FLANN (Fast Library for Approximating Nearest Neighbors)";
        case matcher_K_MEANS_CLUSTERING:	return "K MEANS CLUSTERING";
        default:				            return "UNKNOWN";
    }
}

string Model::getBinarizationName(){
    switch (binarizationMethod){
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
