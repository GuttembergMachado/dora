#include "model.h"

//int Model::classificationModel = model_BAG_OF_FEATURES;
//int Model::featureType = matching_K_MEANS_CLUSTERING;
//int Model::matchingModel = matching_K_MEANS_CLUSTERING;

bool Model::createModel(){

	Log(log_Debug, "model.cpp", "createModel", "   Creating model...");

	try{

		Log(log_Debug, "model.cpp", "createModel", "      Model was created.");
		return true;

	}catch(exception e){
		Log(log_Error, "model.cpp", "createModel",  "      Failed to create model: %s", string(e.what()) ) ;
	}

	return false;

}

bool Model::loadModel(){

	Log(log_Debug, "model.cpp", "loadModel", "   Loading model file...");

	try{

		Log(log_Debug, "model.cpp", "loadModel", "      Model was loaded.");
		return true;

	}catch(exception e){
		Log(log_Error, "model.cpp", "loadModel", "      Failed to load model: %s", string(e.what()) ) ;
	}

	return false;
}

bool Model::saveModel(){

	Log(log_Debug, "model.cpp", "saveModel", "   Saving model file...");

	try{
		//TODO:

		Log(log_Debug, "model.cpp", "saveModel", "      Model was saved.");
		return true;

	}catch(exception e){
		Log(log_Error, "model.cpp", "saveModel", "      Failed to save model: %s", string(e.what()) ) ;
	}

	return false;
}

void Model::dumpModel(){

	Log(log_Debug, "model.cpp", "dumpModel", "   Dumping model file...");

	try{

		//TODO:

		Log(log_Debug, "model.cpp", "dumpModel", "      Model was dumped.");
		return;

	}catch(exception e){
		Log(log_Error, "model.cpp", "dumpModel", "      Failed to dump model: %s", string(e.what()) ) ;
	}

}
