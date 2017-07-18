#include "document.h"
#include "log.h"
#include "helper.h"
#include <algorithm>
#include <fstream>

void Document::loadFile(string f){

	//Anota a hora do inicio do processo
	DWORD s = GetTickCount();

	Log(log_Debug, "document.cpp", "loadFile", "            Loading document '" + f.substr(f.find_last_of("\\") + 1)  + "'...");

	// Anota o nome do documento 
	filename = f;

	//Anota a extensão do arquivo
	type = filename.substr(filename.find_last_of("."));
	transform(type.begin(), type.end(), type.begin(), ::tolower);

	//Anota o tamanho do arquivo
	FILE * fp;
	fopen_s(&fp, filename.c_str(), "rb");
	if (fp == NULL){
		Log(log_Error, "document.cpp", "loadFile", "               File not found");
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
		Log(log_Error, "document.cpp", "loadFile", "               Document type (" + type + ") not supported! ");
		return;
	}

	Log(log_Debug, "document.cpp", "loadFile", "               It took " + getFormatedTime(GetTickCount() - s) + " to load the " + to_string(filesize) + " bytes file.");

}

void Document::saveBinaryFile(string filename){

	//Anota a hora do inicio do processo
	DWORD s = GetTickCount();

	try {
		Log(log_Debug, "document.cpp", "saveBinaryFile", "            Saving binary Mat as '" + filename + "...");
		
		imwrite(filename, binaryMat);

		Log(log_Debug, "document.cpp", "saveBinaryFile", "               It took " + getFormatedTime(GetTickCount() - s) +" to save binary Mat.");
	}
	catch(Exception ex) {
		Log(log_Error, "document.cpp", "saveBinaryFile", "               Unable to save binary Mat.");
	}
}

void Document::saveProjectionFile(string filename){

	//Anota a hora do inicio do processo
	DWORD s = GetTickCount();

	try {
		Log(log_Debug, "document.cpp", "saveProjectionFile", "            Saving projection Mat as '" + filename + "...");

		imwrite(filename, projectionMat);

		Log(log_Debug, "document.cpp", "saveBinaryFile", "               It took " + getFormatedTime(GetTickCount() - s) + " to save projection Mat.");
	}
	catch (Exception ex) {
		Log(log_Error, "document.cpp", "saveProjectionFile", "               Unable to save projection Mat.");
	}
}

void Document::dump(){

	IplImage * org = new IplImage(originalMat);
	IplImage * bin = new IplImage(binaryMat);

	//Faz o dump das imagens
	Log(log_Debug, "document.cpp", "dump", "------------------------------------");
	Log(log_Debug, "document.cpp", "dump", "Dumping document information: ");
	Log(log_Debug, "document.cpp", "dump", "   Filename: " + filename);
	Log(log_Debug, "document.cpp", "dump", "   Type: " +type);
	Log(log_Debug, "document.cpp", "dump", "   Size: " + to_string(filesize));
	Log(log_Debug, "document.cpp", "dump", "   Width: " + to_string(org->width));
	Log(log_Debug, "document.cpp", "dump", "   Height: " + to_string(org->height));
	Log(log_Debug, "document.cpp", "dump", "   Original:");
	Log(log_Debug, "document.cpp", "dump", "      Pixel Depth: " + to_string(org->depth));
	Log(log_Debug, "document.cpp", "dump", "      Channels: " + to_string(org->nChannels));
	Log(log_Debug, "document.cpp", "dump", "      Image Size: " + to_string(org->imageSize) + " bytes");
	Log(log_Debug, "document.cpp", "dump", "   Binary:");
	Log(log_Debug, "document.cpp", "dump", "      Pixel Depth: " + to_string(bin->depth));
	Log(log_Debug, "document.cpp", "dump", "      Channels: " + to_string(bin->nChannels));
	Log(log_Debug, "document.cpp", "dump", "      Image Size: " + to_string(bin->imageSize) + " bytes");

}