#include "sample.h"

void Sample::fixJPEG(string filename){
    Mat m;
    m = imread(filename, CV_LOAD_IMAGE_UNCHANGED);
    imwrite(filename, m);
}

bool Sample::load(string sampleFilename) {

    try{
        Log(log_Detail, "sample.cpp", "load", "      Loading file '%s'...", sampleFilename.c_str());

        filename = sampleFilename;

        originalMat = Mat();

        //Tenta se livrar do "Premature end of JPEG file"
        //fixJPEG(filename);

        //Carrega a imagem exatamente como está no arquivo
        originalMat = imread(filename, CV_LOAD_IMAGE_UNCHANGED);

        type = filename.substr(filename.find_last_of(".") + 1);
        size = fileSize(filename);
        width = originalMat.cols;
        height = originalMat.rows;
        depth = originalMat.dims;

        Log(log_Detail, "sample.cpp", "load", "      File loaded.");
        return true;

    }catch(const std::exception& e){
        Log(log_Error, "sample.cpp", "load",  "         Failed to load file: %s", e.what() ) ;
    }

    Log(log_Warning, "sample.cpp", "load", "      Loading file failed!");
    return false;
}

bool Sample::create_grayscale(){

    try {
        Log(log_Detail, "sample.cpp", "create_grayscale", "      Creating grayscale mat from original mat...");

        cvtColor(originalMat, grayMat, CV_BGR2GRAY);

        if (grayMat.rows > 0 && grayMat.cols > 0 && grayMat.channels() == 1) {
            Log(log_Detail, "sample.cpp", "create_grayscale", "      Grayscale mat created.");
            return true;
        }

    }catch(const std::exception& e){
        Log(log_Error, "sample.cpp", "create_grayscale",  "         Failed to create mat: %s", e.what() ) ;
    }

    Log(log_Warning, "sample.cpp", "create_grayscale", "      Creating grayscale mat failed.");
    return false;
}

bool Sample::create_binary(enumBinarization binMethod){

    try{
        Log(log_Detail, "sample.cpp", "create_binary", "      Creating binary mat from original mat...");

        binarize(grayMat, binaryMat, binMethod);

        if (binaryMat.rows > 0 && binaryMat.cols > 0 && binaryMat.channels() == 1) {
            Log(log_Detail, "sample.cpp", "create_binary", "      Binary mat created.");
            return true;
        }

    }catch(const std::exception& e){
        Log(log_Error, "sample.cpp", "create_binary",  "         Failed to create mat: %s", e.what() ) ;
    }

    Log(log_Warning, "sample.cpp", "create_binary", "      Creating binary mat failed.");
    return false;
}

bool Sample::save(bool original, bool gray, bool binary, bool xCut, bool yCut){

    try{
        Log(log_Debug, "sample.cpp", "save", "      Saving mats as files...");

    }catch(const std::exception& e){
        Log(log_Error, "sample.cpp", "save",  "         Failed to save files: %s", e.what() ) ;
    }

    return false;
}

void Sample::dump(){

    //Faz o dump das imagens
    Log(log_Detail, "sample.cpp", "dump", "            Filename: %s", filename.c_str());
    Log(log_Detail, "sample.cpp", "dump", "            Label: %s", label.c_str());
    Log(log_Detail, "sample.cpp", "dump", "            Type: %s", type.c_str());
    Log(log_Detail, "sample.cpp", "dump", "            Size: %i", size);
    Log(log_Detail, "sample.cpp", "dump", "            Width: %i", width);
    Log(log_Detail, "sample.cpp", "dump", "            Height: %i", height);
    Log(log_Detail, "sample.cpp", "dump", "            Depth: %i", depth);

}

/*
void Document::loadFile(string f){

	//Anota a hora do inicio do processo
	DWORD s = GetTickCount();

	Log(log_Debug, "sample.cpp", "loadFile", "            Loading document '" + f.substr(f.find_last_of("\\") + 1)  + "'...");

	// Anota o nome do documento 
	filename = f;

	//Anota a extensão do arquivo
	type = filename.substr(filename.find_last_of("."));
	transform(type.begin(), type.end(), type.begin(), ::tolower);

	//Anota o tamanho do arquivo
	FILE * fp;
	fopen_s(&fp, filename.c_str(), "rb");
	if (fp == NULL){
		Log(log_Error, "sample.cpp", "loadFile", "               File not found");
		return;
	}
	else{
		fseek(fp, 0, SEEK_END);
		filesize = ftell(fp);
		fclose(fp);
	}

	//Carrega o arquivo
	if (type == ".jpg" || type == ".png"){

		originalMat = imread(filename, CV_LOAD_IMAGE_UNCHANGED);
		binaryMat = imread(filename, CV_LOAD_IMAGE_GRAYSCALE);

		//Aloca o espaço para as outras Mats
		verticalProjectionMat	= Mat();
		horizontalProjectionMat = Mat();
		projectionMat			= Mat();
	}
	else{
		//Por enquanto só existe tratamento para imagens
		Log(log_Error, "sample.cpp", "loadFile", "               Document type (" + type + ") not supported! ");
		return;
	}

	Log(log_Debug, "sample.cpp", "loadFile", "               It took " + getFormatedTime(GetTickCount() - s) + " to load the " + to_string(filesize) + " bytes file.");

}

void Document::saveBinaryFile(string filename){

	//Anota a hora do inicio do processo
	DWORD s = GetTickCount();

	try {
		Log(log_Debug, "sample.cpp", "saveBinaryFile", "            Saving binary Mat as '" + filename + "...");
		
		imwrite(filename, binaryMat);

		Log(log_Debug, "sample.cpp", "saveBinaryFile", "               It took " + getFormatedTime(GetTickCount() - s) +" to save binary Mat.");
	}
	catch(Exception ex) {
		Log(log_Error, "sample.cpp", "saveBinaryFile", "               Unable to save binary Mat.");
	}
}

void Document::saveProjectionFile(string filename){

	//Anota a hora do inicio do processo
	DWORD s = GetTickCount();

	try {
		Log(log_Debug, "sample.cpp", "saveProjectionFile", "            Saving projection Mat as '" + filename + "...");

		imwrite(filename, projectionMat);

		Log(log_Debug, "sample.cpp", "saveBinaryFile", "               It took " + getFormatedTime(GetTickCount() - s) + " to save projection Mat.");
	}
	catch (Exception ex) {
		Log(log_Error, "sample.cpp", "saveProjectionFile", "               Unable to save projection Mat.");
	}
}

 */

