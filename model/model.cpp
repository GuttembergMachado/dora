//
// Guttemberg Machado on 24/07/17.
//
#include "model.h"

bool Model::create(string sampleFolder){

	try{
        int64 startTask = getTick();
        int64 startSubtask;

        vector<string> files;

        //Is the input folder actually an existing folder?
        if(isFolder(sampleFolder)) {

            //Load all files recursivelly
            files = listFiles(sampleFolder);

            //Is there at least one image file?
            if (files.size() > 0){

                samples.clear();

                Log(log_Debug, "model.cpp", "create", "   Loading sample files...");
                startSubtask = getTick();
                for (int i = 0; i < files.size(); i++) {

                    Sample s;

                    //Loads the sample
                    if (s.load(files[i])){

                        //Check if sample is too small
                        if (s.originalMat.cols > minDimension && s.originalMat.rows > minDimension) {

                            //Check if sample needs resizing
                            if(s.originalMat.cols < maxDimension && s.originalMat.rows < maxDimension) {

                                Log(log_Debug, "model.cpp", "create", "      Loaded sample %i...", samples.size());

                                //Creates grayscale mat of the sample
                                if(!s.create_grayscale())
                                    break;

                                //Creates monochromatic mat of the grayscale sample
                                if(!s.create_binary(binarizationMethod))
                                    break;

                                //Creates XYCut images
                                if(!s.create_XYCut())
                                    break;

                                if(saveIntermediateFiles)
                                    s.saveIntermediate(folder, false, true, false, false, false);

                                //Uses the folder as the label
                                s.label = replace(getFolderName(s.filename), sampleFolder, "");
                                samples.push_back(s);
                                s.dump();

                            }else{
                                Log(log_Error, "model.cpp", "create", "      Sample %i is too large! Ignoring it...");
                            }
                        }else{
                            Log(log_Error, "model.cpp", "create", "      Sample is too small! Ignoring it...");
                        }
                    }
                }
                Log(log_Debug, "model.cpp", "create", "      Done. Loaded %i samples out of %i files in %s seconds.", samples.size(), files.size(), getDifString(startSubtask).c_str());

                //Did we manage to load any file at all?
                if(samples.size() > 0) {

                    Log(log_Debug, "model.cpp", "create", "   Initializing modules...");
                    startSubtask = getTick();

                    //Creates the dictionary
                    if(createDictionary()){

                        //Associa o dicionrio  ao matcher
                        startSubtask = getTick();
                        bow.setVocabulary(dictionary);
                        Log(log_Debug, "model.cpp", "create", "      Done. Vocabulary was set in %s seconds.", getDifString(startSubtask).c_str());

                        //Creates the histogramas
                        if(createHistograms(){

                        }


                        //-------------------------------------------------------------------
                        //Salva o arquivo dos histogramas (com os dados para serem treinados)
                        s = GetTickCount();
                        Log(log_Debug, "model.cpp", "saveModel", "   Saving trainingData on file '" + trainingSetFile + "'...");
                        FileStorage fs(trainingSetFile, FileStorage::WRITE);
                        fs << "trainingData" << trainingData;
                        fs.release();
                        Log(log_Debug, "model.cpp", "saveModel", "      It took " + getFormatedTime(GetTickCount() - s) + " to save trainingData on File.");


                        /*

                        //-------------------------------------------------------------------
                        //Verifica se precisa gerar os histogramas
                        if (trainingData.dims == 0){

                            //-------------------------------------------------------------------
                            //Define o dicionário (indicando quais palavras serão consideradas)
                            s = GetTickCount();
                            Log(log_Debug, "Sample.cpp", "createModel", "      Setting vocabulary...");
                            BOWImgDescriptorExtractor bow(extractor, matcher);
                            bow.setVocabulary(dictionary);
                            Log(log_Debug, "Sample.cpp", "createModel", "         It took " + getFormatedTime((GetTickCount() - s)) + " to set vocabulary.");

                            //-------------------------------------------------------------------
                            //Gera um histograma (Palavras do Dicionário X Ocorrencia) para cada imagem
                            s = GetTickCount();
                            Log(log_Debug, "Sample.cpp", "createModel", "      Creating histograms...");
                            Document		doc;
                            PreProcessor	pre;
                            for (int i = 0; i < samples.size(); i++)
                            {
                                Log(log_Debug, "model.cpp", "createModel", "         Sample " + to_string(i + 1) + ": " + samples[i].label + ": " + samples[i].filename.substr(samples[i].filename.find_last_of("\\") + 1));

                                //Carrega o documento
                                doc.loadFile(samples[i].filename);

                                //Faz o pre-processamento da imagem
                                pre.preProcessDoc(doc, alg_BRADLEY, false, false, false);

                                s = GetTickCount();
                                Log(log_Debug, "model.cpp", "createModel", "            Extracting histogram for sample " + to_string(i + 1) + "...");

                                //Get the keypoints
                                detector->detect(doc.binaryMat, keypoints);

                                //Get the descriptors for each keypoint (that is already using the dictionary)
                                bow.compute(doc.binaryMat, keypoints, descriptor);

                                //Adds the descriptor to the trainning array
                                trainingData.push_back(descriptor);

                                Log(log_Debug, "model.cpp", "createModel", "               It took " + getFormatedTime(GetTickCount() - s) + " to extract histogram.");

                                //-------------------------------------------------------------------
                                //Salva o arquivo dos histogramas (com os dados para serem treinados)
                                s = GetTickCount();
                                Log(log_Debug, "model.cpp", "saveModel", "   Saving trainingData on file '" + trainingSetFile + "'...");
                                FileStorage fs(trainingSetFile, FileStorage::WRITE);
                                fs << "trainingData" << trainingData;
                                fs.release();
                                Log(log_Debug, "model.cpp", "saveModel", "      It took " + getFormatedTime(GetTickCount() - s) + " to save trainingData on File.");


                            }
                            Log(log_Debug, "Sample.cpp", "createModel", "      It took " + getFormatedTime((GetTickCount() - s)) + " to create histograms.");

                        }
                        else
                            Log(log_Debug, "model.cpp", "saveModel", "   Creating histograms was not necessary.");

                        //-------------------------------------------------------------------
                        //Verifica se precisa gerar os labels
                        if (labels.size() == 0 || trainingLabel.dims == 0){

                            //-------------------------------------------------------------------
                            //Gera o Mat com os Labels
                            s = GetTickCount();
                            Log(log_Debug, "model.cpp", "saveModel", "   Creating the label Mat...");
                            for (int i = 0; i < samples.size(); i++)
                            {
                                //Verifica se já adicionamos esse Label
                                if (find(labels.begin(), labels.end(), samples[i].label) == labels.end())
                                    labels.push_back(samples[i].label);
                            }

                            for (int i = 0; i < samples.size(); i++)
                            {
                                int label_index = find(labels.begin(), labels.end(), samples[i].label) - labels.begin();
                                trainingLabel.push_back((float)label_index);
                            }
                            Log(log_Debug, "model.cpp", "saveModel", "      It took " + getFormatedTime(GetTickCount() - s) + " to create the labels Mat.");

                            //-------------------------------------------------------------------
                            //Salva o arquivo dos histogramas (com os dados para serem treinados)
                            s = GetTickCount();
                            Log(log_Debug, "model.cpp", "saveModel", "   Saving labels on file '" + trainingSetFile + "'...");
                            FileStorage fs(trainingSetFile, FileStorage::WRITE);
                            fs << "labels" << labels;
                            fs << "trainingLabel" << trainingLabel;
                            fs.release();
                            Log(log_Debug, "model.cpp", "saveModel", "      It took " + getFormatedTime(GetTickCount() - s) + " to save labels on file.");

                        }else
                            Log(log_Debug, "model.cpp", "saveModel", "   Creating labels was not necessary.");

                        //-------------------------------------------------------------------
                        //Treina o Support Vector Macinhe
                        s = GetTickCount();
                        Log(log_Debug, "Sample.cpp", "saveModel", "   Training SVM classifier...");

                        CvSVMParams params;
                        params.kernel_type = CvSVM::RBF;
                        params.svm_type = CvSVM::C_SVC;
                        params.gamma = 0.50625000000000009;
                        params.C = 312.50000000000000;
                        params.term_crit = cvTermCriteria(CV_TERMCRIT_ITER, 100, 0.000001);

                        bool res = svm.train(trainingData, trainingLabel, cv::Mat(), cv::Mat(), params);

                        Log(log_Debug, "Sample.cpp", "saveModel", "      It took " + getFormatedTime((GetTickCount() - s)) + " to train SVM classifier.");

                        Log(log_Debug, "model.cpp", "createModel", "   Creating Model took " + getFormatedTime(GetTickCount() - s) + ".");
                        */



                        //TODO
                        //TODO
                        //TODO
                        //TODO: Continue porting....
                        //TODO
                        //TODO
                        //TODO

                        Log(log_Error, "model.cpp", "create", "   Done creating model in %s seconds.", getDifString(startTask).c_str());
                        return true;

                    }

                } else
                    Log(log_Debug, "model.cpp", "create", "   No samples found.");
            }else
                Log(log_Error, "model.cpp", "create", "   Failed to find sample folder '%s'", sampleFolder.c_str() );
        }else
            Log(log_Error, "model.cpp", "create",  "   Sample folder contains no files.");
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


                //FileStorage fs(trainingSetFile, FileStorage::READ);
                //f//s["dictionary"] >> dictionary;
                //fs["trainingData"] >> trainingData;
                //fs["labels"] >> labels;
                //fs["trainingLabel"] >> trainingLabel;
                //fs.release();





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

	Log(log_Debug, "model.cpp", "save", "   Saving model file...");

	try{

        Log(log_Debug, "model.cpp", "save", "      Saving dictionary on file '" + filename + "'...");
        startSubtask = getTick();
        //Saves
        FileStorage fs(filename, FileStorage::WRITE);
        fs << "dictionary" << dictionary;
        fs.release();
        Log(log_Debug, "model.cpp", "save", "         Done saving dictionary file in %s seconds.",  getDifString(startSubtask).c_str());


        //TODO
        //TODO
        //TODO



		Log(log_Debug, "model.cpp", "save", "      TODO: (NOT IMPLEMENTED): Model was saved.");
		//return true;

	}catch(const std::exception& e){
		Log(log_Error, "model.cpp", "save", "      Failed to save model: %s", e.what() ) ;
	}

	return false;
}

bool Model::initialize(){

    try{

        //TODO: Check what is the optimized size of a dictionary (based on samples, number of labels, etc)
        dictionarySize = 1500;

        //TODO: ALL THE 'initialize functions' from OPENCV changed from version 2.x to 3.x.  CHECK THIS CAREFULLY

        Log(log_Error, "model.cpp", "initialize", "      Initializing trainer module: '" + getModelName() + "'...");
        switch (classificationModel)
        {
            case model_BAG_OF_FEATURES:{
                TermCriteria tc(CV_TERMCRIT_ITER, 10, 0.001);
                int retries = 1;
                int flags = KMEANS_PP_CENTERS;
                trainer = new BOWKMeansTrainer(dictionarySize, tc, retries, flags);
                Log(log_Error, "model.cpp", "initialize", "         Done.");
                break;
            }
            case model_PROJECTION:{
                Log(log_Error, "model.cpp", "initialize", "         MODEL '" + getModelName() + "' NOT IMPLEMENTED.");
                return false;
            }

        }

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
                Log(log_Error, "model.cpp", "initialize", "         FEATURE '" + getFeatureName() + "' NOT IMPLEMENTED.");
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
                Log(log_Error, "model.cpp", "initialize", "         MATCHER '" + getMatcherName() + "' NOT IMPLEMENTED.");
                return false;
        }

        //TODO: Analisar isso aqui
        //Associa um matcher ao extractor
        BOWImgDescriptorExtractor bow(extractor, matcher);

        return true;

    }catch(const std::exception& e){
        Log(log_Error, "model.cpp", "create",  "   Error creating model: %s", e.what()) ;
    }

}

bool Model::loadDictionary() {

    int64 startTask = getTick();

    Log(log_Debug, "model.cpp", "loadDictionary", "      Loading existing dictionary...");

    //Load dictionary from file
    FileStorage fs(filename, FileStorage::READ);
    fs["dictionary"] >> dictionary;
    fs.release();

    Log(log_Debug, "model.cpp", "loadDictionary", "         Done. Dictionary was loaded in %s seconds.", getDifString(startTask).c_str());
    return true;

}

bool Model::createDictionary() {

    int64 startTask = getTick();
    int64 startSubtask;

    Log(log_Debug, "model.cpp", "createDictionary", "      Creating a new dictionary based on the %i samples found...", samples.size());
    startSubtask = getTick();

    vector<KeyPoint> keypoints;
    Mat descriptor;

    for (int i = 0; i < samples.size(); i++) {

        Mat m = samples[i].grayMat;

        //Check if the original mat exists
        if (m.rows > 1 && m.cols > 1) {

            Log(log_Detail, "model.cpp", "createDictionary", "         sample %05d ('%s'):", (i + 1),samples[i].filename.c_str());

            //Get the keypoints
            Log(log_Detail, "model.cpp", "createDictionary", "            Extracting features (key points)...");
            detector->detect(m, keypoints);

            //Get the descriptors for each keypoint
            Log(log_Detail, "model.cpp", "createDictionary", "            Computing descriptors from features (key points)...");
            extractor->compute(m, keypoints, descriptor);

            //Adds the descriptor to the trainer
            Log(log_Detail, "model.cpp", "createDictionary", "            Adding descriptors...");
            if (!descriptor.empty())
                trainer->add(descriptor);

        } else {
            Log(log_Warning, "model.cpp", "createDictionary","         sample %05d ('%s') ignored because original mat was not found...", (i + 1), samples[i].filename.c_str());
        }
    }
    Log(log_Debug, "model.cpp", "createDictionary", "      Done loading all %i samples in %s seconds.", samples.size(), getDifString(startSubtask).c_str());

    //Did processing the samples find anything usefull?
    if (trainer->descriptorsCount() > 0){

        Log(log_Debug, "model.cpp", "createDictionary", "      Clustering features (choosing centroids as words) out of all %i descriptors found...", trainer->descriptorsCount());
        startSubtask = getTick();

        //Cluster (use the centroid of each cluster as the words of the dictionary)
        dictionary = trainer->cluster();

        if(dictionary.dims > 0){
            Log(log_Debug, "model.cpp", "createDictionary", "         Done. Dictionary was created and initialized  in %s seconds.", getDifString(startTask).c_str());
            return true;
        }

        Log(log_Debug, "model.cpp", "createDictionary", "         ERROR: Failed to create dictionary.");

    }else
        Log(log_Error, "model.cpp", "createDictionary", "         ERROR: Failed to cluster because no descriptors were found!");

    return false;

}

bool Model::createHistograms() {


    int64 startTask = getTick();
    int64 startSubtask;

    Log(log_Debug, "model.cpp", "createHistograms", "      Creating a new histograms based on the %i samples found...", samples.size());
    startSubtask = getTick();

    vector<KeyPoint> keypoints;
    Mat descriptor;

    for (int i = 0; i < samples.size(); i++) {

        Mat m = samples[i].binaryMat;

        //Check if the original mat exists
        if (isMatValid(m)) {

            Log(log_Detail, "model.cpp", "createHistograms", "         sample %05d ('%s'):", (i + 1),samples[i].filename.c_str());

            //Get the keypoints
            Log(log_Detail, "model.cpp", "createHistograms", "            Extracting features (key points)...");
            detector->detect(m, keypoints);

            Log(log_Detail, "model.cpp", "createHistograms", "            Computing descriptors from features (key points)...");
            bow.compute(m, keypoints, descriptor);

            //Adds the descriptor to the trainning array
            Log(log_Detail, "model.cpp", "createHistograms", "            Adding descriptors...");
            trainingData.push_back(descriptor);

            if(trainingData.size>0){
                Log(log_Debug, "model.cpp", "createDictionary", "         Done. Training data was created in %s seconds.", getDifString(startTask).c_str());
                return true;
            }

            Log(log_Warning, "model.cpp", "createHistograms","         sample %05d ('%s') ignored because original mat was not found...", (i + 1), samples[i].filename.c_str());


        } else {
            Log(log_Warning, "model.cpp", "createHistograms","         sample %05d ('%s') ignored because original mat was not found...", (i + 1), samples[i].filename.c_str());
        }
    }
    Log(log_Debug, "model.cpp", "createHistograms", "      Done loading all %i samples in %s seconds.", samples.size(), getDifString(startSubtask).c_str());

    //Did processing the samples find anything usefull?
    if (trainer->descriptorsCount() > 0){

        Log(log_Debug, "model.cpp", "createHistograms", "      Clustering features (choosing centroids as words) out of all %i descriptors found...", trainer->descriptorsCount());
        startSubtask = getTick();

        //Cluster (use the centroid of each cluster as the words of the dictionary)
        dictionary = trainer->cluster();

        if(dictionary.dims > 0){
            Log(log_Debug, "model.cpp", "createHistograms", "         Done. Dictionary was created and initialized  in %s seconds.", getDifString(startTask).c_str());
            return true;
        }

        Log(log_Debug, "model.cpp", "createHistograms", "         ERROR: Failed to create dictionary.");

    }else
        Log(log_Error, "model.cpp", "createHistograms", "         ERROR: Failed to cluster because no descriptors were found!");

    return false;

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
