//
// Guttemberg Machado on 24/07/17.
//
#include "sample.h"

Sample::Sample(){

    //Initialize the internal variables
    mLabel="";
    mFilename="";
    mType="";
    mSize=0;
    mWidth=0;
    mHeight=0;
    mDepth=0;

};

bool Sample::load(string filename, string label, bool fixBrokenJPG) {

    try {
        Log(log_Detail, "sample.cpp", "load", "      Loading file '%s'...", filename.c_str());

        mFilename = filename;
        mLabel = label;

        //Loads an unchanged mat from the image file
        mOriginalMat = imread(mFilename, CV_LOAD_IMAGE_UNCHANGED);

        //Do we have a mat?
        if (isMatValid(mOriginalMat)) {

            if (fixBrokenJPG) {
                Log(log_Detail, "sample.cpp", "load", "      Saving file again to avoid the 'Premature end of JPEG file' exception...");
                save();
            }

            //Stores some sample information to speed up debugging when I need to...
            mType = filename.substr(mFilename.find_last_of(".") + 1);
            mSize = fileSize(mFilename);
            mWidth = mOriginalMat.cols;
            mHeight = mOriginalMat.rows;
            mDepth = mOriginalMat.dims;

            Log(log_Detail, "sample.cpp", "load", "      File loaded.");
            return true;

        } else {
            Log(log_Error, "sample.cpp", "load", "      File was loaded but image mat is invalid!");
        }

    } catch (const std::exception &e) {
        Log(log_Error, "sample.cpp", "load", "         Failed to load file: %s", e.what());
    }

    return false;

}

string Sample::getLabel(){
    return mLabel;
}

string Sample::getFilename(){
    return mFilename;
}

string Sample::getType(){
    return mType;
}

int Sample::getSize(){
    return mSize;
}

int Sample::getWidth(){
    return mWidth;
}

int Sample::getHeight(){
    return mHeight;
}

int Sample::getDepth(){
    return mDepth;
}




/*
bool Sample::preProcess(){

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

}

bool Sample::extractFeatures(){

    Log(log_Detail, "model.cpp", "createDictionary", "            Extracting features...");
    detector->detect(samples[i].binaryMat, keypoints);
    if(keypoints.size() > 0){
        Log(log_Detail, "model.cpp", "createDictionary", "            Computing descriptors from features...");
        extractor->compute(samples[i].binaryMat, keypoints, descriptors);
        if (descriptors.cols > 0){
            Log(log_Detail, "model.cpp", "createDictionary", "            Saving descriptors...");
            trainer->add(descriptors);
        }else
            Log(log_Error, "model.cpp", "prepareTrainingSet", "               Failed: No descriptors were found on sample %i ('%s'), even though %i keypoints were found...", (i + 1), keypoints.size(), samples[i].filename.c_str() );
    }else
        Log(log_Error, "model.cpp", "prepareTrainingSet", "               Failed: No features (key points) were found on sample %i ('%s')...", (i + 1), samples[i].filename.c_str() );


}


bool Sample::createWorkMat(int width, int height){

    try{
        Log(log_Detail, "sample.cpp", "createWorkMat", "      Creating a resized (W:%i, H%i) work mat from original mat...", width, height);

        if(isMatValid(originalMat)) {

            //INTER_NEAREST  - a nearest-neighbor interpolation
            //INTER_LINEAR   - a bilinear interpolation (used by default)
            //INTER_AREA     - a resampling using pixel area relation. It may be a preferred method for image decimation, as it gives moire’-free results. But when the image is zoomed, it is similar to the INTER_NEAREST method
            //INTER_CUBIC    - a bicubic interpolation over 4x4 pixel neighborhood
            //INTER_LANCZOS4 - a Lanczos interpolation over 8x8 pixel neighborhood
            //
            //To shrink an image, it will generally look best with CV_INTER_AREA interpolation,
            //To enlarge an image, it will generally look best with CV_INTER_CUBIC (slow) or CV_INTER_LINEAR (faster but still looks OK).

            //TODO:  Fazer o resize respeitando o aspect ratio
            // Size s = Size(width, height);
            Size s = Size(originalMat.cols, originalMat.rows);

            resize(originalMat, workMat, s, 0, 0, INTER_CUBIC);

            if(isMatValid(workMat)){
                Log(log_Detail, "sample.cpp", "createWorkMat", "      Working mat created.");
                return true;
            }
        }

    }catch(const std::exception& e){
        Log(log_Error, "sample.cpp", "createWorkMat",  "         Failed to create mat: %s", e.what() ) ;
    }

    Log(log_Warning, "sample.cpp", "createWorkMat", "      Creating working mat failed.");
    return false;
}

bool Sample::createGrayscaleMat(){

    try {
        Log(log_Detail, "sample.cpp", "createGrayscaleMat", "      Creating grayscale mat from original mat...");

        if(isMatValid(workMat)){

            //convert the originalMat to grayscale (ignores it if is already grayscale). This functions combines RGB values with weights R=, G= and B=)
            cvtColor(workMat, grayMat, CV_BGR2GRAY);

            if(isMatValid(grayMat)){
                Log(log_Detail, "sample.cpp", "createGrayscaleMat", "      Grayscale mat created.");
                return true;
            }
        }

    }catch(const std::exception& e){
        Log(log_Error, "sample.cpp", "createGrayscaleMat",  "         Failed to create mat: %s", e.what() ) ;
    }

    Log(log_Warning, "sample.cpp", "createGrayscaleMat", "      Creating grayscale mat failed.");
    return false;
}

bool Sample::createBinaryMat(enumBinarization binMethod){

    try{
        Log(log_Detail, "sample.cpp", "createBinaryMat", "      Creating binary mat from gray mat...");

        if(isMatValid(grayMat)) {

            //converts the gray mat to a black and white one
            binarize(grayMat, binaryMat, binMethod);

            if(isMatValid(binaryMat)){
                Log(log_Detail, "sample.cpp", "createBinaryMat", "      Binary mat created.");
                return true;
            }
        }

    }catch(const std::exception& e){
        Log(log_Error, "sample.cpp", "createBinaryMat",  "         Failed to create mat: %s", e.what() ) ;
    }

    Log(log_Warning, "sample.cpp", "createBinaryMat", "      Creating binary mat failed.");
    return false;
}

bool Sample::createXYCutMat(){

    try{
        Log(log_Detail, "sample.cpp", "createXYCutMat", "      Creating XYCut mats from binary mat...");

        if(isMatValid(binaryMat)) {

            if(getXCut(binaryMat, xCutMat));
                if(getYCut(binaryMat, yCutMat));
                    if(getXYCut(binaryMat, XYCutMat));

            if(isMatValid(xCutMat) && isMatValid(yCutMat) && isMatValid(XYCutMat) ){
                Log(log_Detail, "sample.cpp", "createXYCutMat", "      Done. All 3 XYCut mat were created.");
                return true;
            }
        }

    }catch(const std::exception& e){
        Log(log_Error, "sample.cpp", "createXYCutMat",  "         Failed to create XYCut mats: %s", e.what() ) ;
    }

    Log(log_Warning, "sample.cpp", "createXYCutMat", "      Creating XYCut mats failed.");
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

*/

bool Sample::save(){

    try{
        Log(log_Debug, "sample.cpp", "save", "      Saving original mat as '%s'...", mFilename.c_str());

        if(isMatValid(mOriginalMat)) {

            //saves mat to file
            imwrite(mFilename, mOriginalMat);

            Log(log_Debug, "sample.cpp", "save", "         Done.");
            return true;

        }else
            Log(log_Error, "sample.cpp", "create_binary", "      Original mat is invalid.");

    }catch(const std::exception& e){
        Log(log_Error, "sample.cpp", "save",  "      Failed to save original mat: %s", e.what() ) ;
    }

    return false;
}
