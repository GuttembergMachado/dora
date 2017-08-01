//
// Guttemberg Machado on 24/07/17.
//
#include "sample.h"

bool fixJPEGError = false;

bool Sample::isMatValid(Mat m){

    bool bRet = (m.rows > 0 && m.cols > 0 && m.channels() > 0 && !m.empty());
    return bRet;

}

bool Sample::load(string sampleFilename) {

    try {
        Log(log_Detail, "sample.cpp", "load", "      Loading file '%s'...", sampleFilename.c_str());

        filename = sampleFilename;

        //Loads an unchanged mat from the image file
        originalMat = imread(filename, CV_LOAD_IMAGE_UNCHANGED);

        //Do we have a mat?
        if(isMatValid(originalMat)){

            //Some of the pre-processing I did while I was manually classifing the first trainning set end up
            //corrupting some of the jpgs (probably due to ignoring the end bytes of the file). Such corrputed
            //files were causing opencv's imread functino to output an error ("Premature end of JPEG file"),
            //but would load the image anyway.
            //By saving the file using opencv's imwrite function we garantee it's properly written to disk
            if (fixJPEGError && isMatValid(originalMat)){
                Log(log_Detail, "sample.cpp", "load",  "      Saving file again to avoid the 'Premature end of JPEG file' exception...");
                save();
            }

            //Stores some sample information to speed up debugging when I need to...
            type = filename.substr(filename.find_last_of(".") + 1);
            size = fileSize(filename);
            width = originalMat.cols;
            height = originalMat.rows;
            depth = originalMat.dims;

            Log(log_Detail, "sample.cpp", "load", "      File loaded.");
            return true;
        }

    }catch(const std::exception& e){
        Log(log_Error, "sample.cpp", "load",  "         Failed to load file: %s", e.what() ) ;
    }

    Log(log_Warning, "sample.cpp", "load", "      Loading file failed!");
    return false;
}

bool Sample::create_grayscale(){

    try {
        Log(log_Detail, "sample.cpp", "create_grayscale", "      Creating grayscale mat from original mat...");

        if(isMatValid(originalMat)){

            //convert the originalMat to grayscale (ignores it if is already grayscale). This functions combines RGB values with weights R=, G= and B=)
            cvtColor(originalMat, grayMat, CV_BGR2GRAY);

            if(isMatValid(grayMat)){
                Log(log_Detail, "sample.cpp", "create_grayscale", "      Grayscale mat created.");
                return true;
            }
        }

    }catch(const std::exception& e){
        Log(log_Error, "sample.cpp", "create_grayscale",  "         Failed to create mat: %s", e.what() ) ;
    }

    Log(log_Warning, "sample.cpp", "create_grayscale", "      Creating grayscale mat failed.");
    return false;
}

bool Sample::create_binary(enumBinarization binMethod){

    try{
        Log(log_Detail, "sample.cpp", "create_binary", "      Creating binary mat from gray mat...");

        if(isMatValid(grayMat)) {

            //converts the gray mat to a black and white one
            binarize(grayMat, binaryMat, binMethod);

            if(isMatValid(binaryMat)){
                Log(log_Detail, "sample.cpp", "create_binary", "      Binary mat created.");
                return true;
            }
        }

    }catch(const std::exception& e){
        Log(log_Error, "sample.cpp", "create_binary",  "         Failed to create mat: %s", e.what() ) ;
    }

    Log(log_Warning, "sample.cpp", "create_binary", "      Creating binary mat failed.");
    return false;
}

bool Sample::create_XYCut(){

    try{
        Log(log_Detail, "sample.cpp", "create_XYCut", "      Creating XYCut mats from binary mat...");

        if(isMatValid(binaryMat)) {

            if(getXCut(binaryMat, xCutMat));
                if(getYCut(binaryMat, yCutMat));
                    if(getXYCut(binaryMat, XYCutMat));

            if(isMatValid(xCutMat) && isMatValid(yCutMat) && isMatValid(XYCutMat) ){
                Log(log_Detail, "sample.cpp", "create_XYCut", "      Done. All 3 XYCut mat were created.");
                return true;
            }
        }

    }catch(const std::exception& e){
        Log(log_Error, "sample.cpp", "create_XYCut",  "         Failed to create XYCut mats: %s", e.what() ) ;
    }

    Log(log_Warning, "sample.cpp", "create_XYCut", "      Creating XYCut mats failed.");
    return false;
}

bool Sample::save(){

    try{
        Log(log_Debug, "sample.cpp", "save", "      Saving original mat...");

        if(isMatValid(originalMat)) {

            //saves mat to file
            //imwrite(filename, originalMat);

            Log(log_Debug, "sample.cpp", "save", "         Done saving original mat...");
            return true;

        }

    }catch(const std::exception& e){
        Log(log_Error, "sample.cpp", "save",  "         Failed to save original mat: %s", e.what() ) ;
    }

    Log(log_Warning, "sample.cpp", "create_binary", "      Saving original mat failed.");
    return false;
}

void Sample::saveIntermediate(string folder, bool gray, bool binary, bool xCut, bool yCut, bool XYCut){

    try{

        string tempFilename;

        if(gray && isMatValid(grayMat)){
            tempFilename = folder + "/temp/gray_" + getFileName(filename);
            Log(log_Detail, "sample.cpp", "saveIntermediate", "      Saving gray mat as '%s'...", tempFilename.c_str());
            imwrite(tempFilename, grayMat);
            Log(log_Detail, "sample.cpp", "saveIntermediate", "         Done saving gray mat.");
        }

        if(binary && isMatValid(binaryMat)){
            tempFilename = folder + "/temp/binary_" + getFileName(filename);
            Log(log_Detail, "sample.cpp", "saveIntermediate", "      Saving binary mat as '%s'...", tempFilename.c_str());
            imwrite(tempFilename, binaryMat);
            Log(log_Detail, "sample.cpp", "saveIntermediate", "         Done saving binary mat.");
        }

        if(xCut && isMatValid(xCutMat)){
            tempFilename = folder + "/temp/xcut_" + getFileName(filename);
            Log(log_Detail, "sample.cpp", "saveIntermediate", "      Saving xCut mat as '%s'...", tempFilename.c_str());
            imwrite(tempFilename, xCutMat);
            Log(log_Detail, "sample.cpp", "saveIntermediate", "         Done saving xCut mat.");
        }

        if(yCut && isMatValid(yCutMat)){
            tempFilename = folder + "/temp/ycut_" + getFileName(filename);
            Log(log_Detail, "sample.cpp", "saveIntermediate", "      Saving yCut mat as '%s'...", tempFilename.c_str());
            imwrite(tempFilename, yCutMat);
            Log(log_Detail, "sample.cpp", "saveIntermediate", "         Done saving gray mat.");
        }

        if(XYCut && isMatValid(XYCutMat)){
            tempFilename = folder + "/temp/xycut_" + getFileName(filename);
            Log(log_Detail, "sample.cpp", "saveIntermediate", "      Saving XYCut mat as '%s'...", tempFilename.c_str());
            imwrite(tempFilename, XYCutMat);
            Log(log_Detail, "sample.cpp", "saveIntermediate", "         Done saving XYCut mat.");
        }

    }catch(const std::exception& e){
        Log(log_Error, "sample.cpp", "save",  "         Failed to save files: %s", e.what() ) ;
    }

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
    if(isMatValid(grayMat))
        Log(log_Detail, "sample.cpp", "dump", "            grayMat: created (%i cols, %i rows, %i channels).", grayMat.rows, grayMat.cols, grayMat.channels());
    if(isMatValid(binaryMat))
        Log(log_Detail, "sample.cpp", "dump", "            binaryMat: created (%i cols, %i rows, %i channels).", binaryMat.rows, binaryMat.cols, binaryMat.channels());
    if(isMatValid(xCutMat))
        Log(log_Detail, "sample.cpp", "dump", "            xCutMat: created (%i cols, %i rows, %i channels).", xCutMat.rows, xCutMat.cols, xCutMat.channels());
    if(isMatValid(yCutMat))
        Log(log_Detail, "sample.cpp", "dump", "            yCutMat: created (%i cols, %i rows, %i channels).", yCutMat.rows, yCutMat.cols, yCutMat.channels());
    if(isMatValid(XYCutMat))
        Log(log_Detail, "sample.cpp", "dump", "            XYCut: created (%i cols, %i rows, %i channels).", XYCutMat.rows, XYCutMat.cols, XYCutMat.channels());

}

