//
// Guttemberg Machado on 24/07/17.
//
#include "model.h"

bool Model::create(string sampleFolder){

	try{
        int64 startTask = getTick();
        int64 startSubtask;

        vector<string> files;

        //Verifica se é uma pasta
        if(isFolder(sampleFolder)) {

            //Carrega os arquivos da pasta
            files = listFiles(sampleFolder);

            //Verifica se tem ao menos um arquivo
            if (files.size() > 0){

                //Zera os samples
                samples.clear();

                Log(log_Debug, "model.cpp", "create", "   Loading sample files...");
                startSubtask = getTick();
                for (int i = 0; i < files.size(); i++) {

                    Sample s;

                    //Carrega a imagem
                    if (s.load(files[i])){
                        s.label = replace(getFolderName(s.filename), sampleFolder, "");
                        s.dump();
                        samples.push_back(s);
                    }

                }
                Log(log_Debug, "model.cpp", "create", "      Done. Loaded %i samples in %s seconds.", samples.size() , getDifString(startSubtask).c_str());


                Log(log_Debug, "model.cpp", "create", "   Creating auxiliary mats...");
                startSubtask = getTick();
                int i;
                for (i = 0; i < samples.size(); i++) {

                    //Creates grayscale mat of the sample
                    if(!samples[i].create_grayscale())
                        break;

                    //Creates monochromatic mat of the grayscale sample
                    if(!samples[i].create_binary(binarizationMethod))
                        break;

                }
                if(i == samples.size()) {
                    Log(log_Debug, "model.cpp", "create", "         Auxiliary mats were created in %s seconds.", getDifString(startSubtask).c_str());

                    Log(log_Debug, "model.cpp", "create", "   Initializing modules...");
                    startSubtask = getTick();
                    if (initializeModules()) {
                        Log(log_Debug, "model.cpp", "create", "      Done. Modules initialized in %s seconds.", getDifString(startSubtask).c_str());

                        Log(log_Debug, "model.cpp", "create", "   Initializing dictionary..."); startSubtask = getTick();
                        if (initializeDictionary()) {
                            Log(log_Debug, "model.cpp", "create", "      Done. Dictionary was created in %s seconds.", getDifString(startSubtask).c_str());


                            //TODO
                            //TODO
                            //TODO
                            //TODO: Contiuar o port....
                            //TODO
                            //TODO
                            //TODO


                            Log(log_Error, "model.cpp", "create", "   Model was created successuflly in %s seconds.", getDifString(startTask).c_str());
                            return true;

                        } else
                            Log(log_Error, "model.cpp", "create", "   Failed to initialize dictionary!");
                    } else
                        Log(log_Error, "model.cpp", "create", "   Failed to initialize modules!");
                } else
                    Log(log_Debug, "model.cpp", "create", "   Failed to create auxiliary mat for sample %i.", (i+1));
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

            //Lê o modelo
            //svm.load(filename.c_str());
            //
            string trainingSetFile = filename + "_trainingSetFile.xml";

            if (isFile(trainingSetFile)){

                FileStorage fs(trainingSetFile, FileStorage::READ);
                fs["dictionary"] >> dictionary;
                //fs["labels"] >> labels;
                fs.release();

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

	Log(log_Debug, "model.cpp", "save", "   Saving model file...");

	try{
		//TODO:

		Log(log_Debug, "model.cpp", "save", "      TODO: (NOT IMPLEMENTED): Model was saved.");
		//return true;

	}catch(const std::exception& e){
		Log(log_Error, "model.cpp", "save", "      Failed to save model: %s", e.what() ) ;
	}

	return false;
}

bool Model::initializeModules(){

    try{
        //Valores defaults
        dictionarySize = 1500;

        //Inicializa os objetos do modelo
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

        //Inicializa os objetos das features
        Log(log_Error, "model.cpp", "initialize", "      Initializing detector module: '" + getFeatureName() + "'...");
        switch (featureType)
        {
            case feature_SIFT: {
                detector =  SiftFeatureDetector::create();      //makePtr<SiftFeatureDetector>();       // was: = new SiftFeatureDetector();
                extractor = SiftDescriptorExtractor::create();  //makePtr<SiftDescriptorExtractor>()    // was: = new SiftDescriptorExtractor();
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

        //Inicializa os objetos do Matcher
        Log(log_Error, "model.cpp", "initialize", "      Initializing matcher module: '" + getMatcherName() + "'...");
        switch (matcherModel)
        {
            case matcher_FLANN: {
                matcher = new FlannBasedMatcher();
                Log(log_Error, "model.cpp", "initialize", "         Done.");
                break;
            }
            case matcher_K_MEANS_CLUSTERING:
            case matcher_BRUTE_FORCE:
                Log(log_Error, "model.cpp", "initialize", "         MATCHER '" + getMatcherName() + "' NOT IMPLEMENTED.");
                return false;
        }

        return true;

    }catch(const std::exception& e){
        Log(log_Error, "model.cpp", "create",  "   Error creating model: %s", e.what()) ;
    }

}

bool Model::initializeDictionary() {

    int64 startTask = getTick();
    int64 startSubtask;

    //-------------------------------------------------------------------
    //Verifica se é para carregar o arquivo de treinamento
    if (isFile(filename)){

        Log(log_Debug, "model.cpp", "initializeDictionary", "      Found an existing model file ('%s'). Loading dictionary from it...");
        startSubtask = getTick();

        //Lê o dicionário do arquivo
        FileStorage fs(filename, FileStorage::READ);
        fs["dictionary"] >> dictionary;
        //fs["trainingData"] >> trainingData;
        //fs["labels"] >> labels;
        //fs["trainingLabel"] >> trainingLabel;
        fs.release();

        Log(log_Debug, "model.cpp", "create", "      Done. Dictionary was initialized from model file in %s seconds.",  getDifString(startSubtask).c_str());
        return true;

    }else {

        Log(log_Debug, "model.cpp", "initializeDictionary", "      Creating a dictionary based on the %i samples found...", samples.size());
        startSubtask = getTick();

        vector<KeyPoint> keypoints;
        Mat descriptor;

        for (int i = 0; i < samples.size(); i++) {

            Mat m = samples[i].grayMat;

            //Check if the original mat exists
            if (m.rows > 1 && m.cols > 1) {

                Log(log_Detail, "model.cpp", "initializeDictionary", "         sample %05d ('%s'):", (i + 1),samples[i].filename.c_str());

                //Get the keypoints
                Log(log_Detail, "model.cpp", "initializeDictionary", "            Extracting features (key points)...");
                detector->detect(m, keypoints);

                //Get the descriptors for each keypoint
                Log(log_Detail, "model.cpp", "initializeDictionary", "            Computing descriptors from features (key points)...");
                extractor->compute(m, keypoints, descriptor);

                //Adds the descriptor to the trainer
                Log(log_Detail, "model.cpp", "initializeDictionary", "            Adding descriptors...");
                trainer->add(descriptor);

            } else {
                Log(log_Warning, "model.cpp", "initializeDictionary","         sample %05d ('%s') ignored because original mat was not found...", (i + 1), samples[i].filename.c_str());
            }
        }
        Log(log_Debug, "model.cpp", "create", "      Done. %i samples processed in %s seconds.", samples.size(), getDifString(startSubtask).c_str());

        //Faz o clustering, para usar o centro de cada clusters como dicionário
        Log(log_Debug, "model.cpp", "initializeDictionary", "      Clustering features (choosing centroids as words) out of %i descriptors...", trainer->descriptorsCount());
        if (trainer->descriptorsCount() > 0){
            startSubtask = getTick();
            dictionary = trainer->cluster();

            Log(log_Debug, "model.cpp", "initializeDictionary", "         Done. Features were clustered and descriptors extracted in %s seconds.", getDifString(startSubtask).c_str());

            //Salva o dicionário para facilitar os próximos passos
            Log(log_Debug, "model.cpp", "initializeDictionary", "      Saving dictionary on file '" + filename + "'...");

            startSubtask = getTick();
            FileStorage fs(filename, FileStorage::WRITE);
            fs << "dictionary" << dictionary;
            fs.release();

            Log(log_Debug, "model.cpp", "initializeDictionary", "         Done.");

            Log(log_Debug, "model.cpp", "initializeDictionary", "      Done. Dictionary was created in %s seconds.",  getDifString(startTask).c_str());
            return true;

        }else
            Log(log_Error, "model.cpp", "initializeDictionary", "         ERROR: Failed to cluster because no descriptors were found!");
    }

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

/*

             //-------------------------------------------------------------------
            //Verifica se precisa gerar os histogramas
            if (trainingData.dims == 0){

                //-------------------------------------------------------------------
                //Define o dicionário (indicando quais palavras serão consideradas)
                Log(log_Debug, "Sample.cpp", "createModel", "      Setting vocabulary...");
                //BOWImgDescriptorExtractor bow(extractor, matcher);
                //bow.setVocabulary(dictionary);
                Log(log_Debug, "Sample.cpp", "createModel", "         Done.");

                //-------------------------------------------------------------------
                //Gera um histograma (Palavras do Dicionário X Ocorrencia) para cada imagem
                Log(log_Debug, "Sample.cpp", "createModel", "      Creating histograms...");
                //Document		doc;
                //PreProcessor	pre;
                for (int i = 0; i < samples.size(); i++)
                {
                    Log(log_Debug, "model.cpp", "create", "         Sample " + to_string(i + 1) + ": " + samples[i].label + ": " + samples[i].filename.substr(samples[i].filename.find_last_of("\\") + 1));

                    //Carrega o documento
                    //doc.loadFile(samples[i].filename);
                    //
                    //Faz o pre-processamento da imagem
                    //pre.preProcessDoc(doc, alg_BRADLEY, false, false, false);
                    //
                    Log(log_Debug, "model.cpp", "create", "            Extracting histogram for sample " + to_string(i + 1) + "...");
                    //
                    //Get the keypoints
                    //detector->detect(doc.binaryMat, keypoints);
                    //
                    //Get the descriptors for each keypoint (that is already using the dictionary)
                    //bow.compute(doc.binaryMat, keypoints, descriptor);
                    //
                    //Adds the descriptor to the trainning array
                    //trainingData.push_back(descriptor);
                    //
                    Log(log_Debug, "model.cpp", "create", "               Done.");

                    //-------------------------------------------------------------------
                    //Salva o arquivo dos histogramas (com os dados para serem treinados)
                    Log(log_Debug, "model.cpp", "saveModel", "   Saving trainingData on file '" + trainingSetFile + "'...");
                    //FileStorage fs(trainingSetFile, FileStorage::WRITE);
                    //fs << "trainingData" << trainingData;
                    //fs.release();
                    Log(log_Debug, "model.cpp", "saveModel", "      Done.");


                }
                Log(log_Debug, "model.cpp", "create", "      Done.");

            }
            else
                Log(log_Debug, "model.cpp", "saveModel", "   Creating histograms was not necessary.");

            //-------------------------------------------------------------------
            //Verifica se precisa gerar os labels
            if (labels.size() == 0 || trainingLabel.dims == 0){

                //-------------------------------------------------------------------
                //Gera o Mat com os Labels
                Log(log_Debug, "model.cpp", "saveModel", "   Creating the label Mat...");
                //for (int i = 0; i < samples.size(); i++)
                //{
                //    //Verifica se já adicionamos esse Label
                //    if (find(labels.begin(), labels.end(), samples[i].label) == labels.end())
                //        labels.push_back(samples[i].label);
                //}

                //for (int i = 0; i < samples.size(); i++)
                //{
                //    int label_index = find(labels.begin(), labels.end(), samples[i].label) - labels.begin();
                //    trainingLabel.push_back((float)label_index);
                //}
                Log(log_Debug, "model.cpp", "saveModel", "      Done.");

                //-------------------------------------------------------------------
                //Salva o arquivo dos histogramas (com os dados para serem treinados)
                Log(log_Debug, "model.cpp", "saveModel", "   Saving labels on file '" + trainingSetFile + "'...");
                //FileStorage fs(trainingSetFile, FileStorage::WRITE);
                //fs << "labels" << labels;
                //fs << "trainingLabel" << trainingLabel;
                //fs.release();
                Log(log_Debug, "model.cpp", "saveModel", "      Done.");

            }else
                Log(log_Debug, "model.cpp", "saveModel", "   Creating labels was not necessary.");

            //-------------------------------------------------------------------
            //Treina o Support Vector Macinhe
            Log(log_Debug, "model.cpp", "create", "   Training SVM classifier...");

            //CvSVMParams params;
            //params.kernel_type = CvSVM::RBF;
            //params.svm_type = CvSVM::C_SVC;
            //params.gamma = 0.50625000000000009;
            //params.C = 312.50000000000000;
            //params.term_crit = cvTermCriteria(CV_TERMCRIT_ITER, 100, 0.000001);
            //
            //bool res = svm.train(trainingData, trainingLabel, cv::Mat(), cv::Mat(), params);
            //
            Log(log_Debug, "model.cpp", "create", "      Done.");
*/
