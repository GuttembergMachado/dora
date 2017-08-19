//
// Guttemberg Machado on 24/07/17.
//
#include "sample.h"

Sample::Sample(){

    //Initialize the internal variables
    mLabel="";
    mFilename="";

};

bool Sample::load(string filename, string label, bool fixBrokenJPG) {

    try {
        Log(log_Detail, "sample.cpp", "load", "      Loading file '%s'...", filename.c_str());

        mFilename = filename;
        mLabel = label;

        //Loads an unchanged mat from the image file
        originalMat = imread(mFilename, CV_LOAD_IMAGE_UNCHANGED);

        //Do we have a mat?
        if (isMatValid(originalMat)) {

            if (fixBrokenJPG) {
                Log(log_Detail, "sample.cpp", "load", "      Saving file again to avoid the 'Premature end of JPEG file' exception...");
                saveMat(originalMat, mFilename);
            }

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

bool Sample::preProcess(int minDimension, int maxDimension, int desiredWidth, int desiredHeight, enumBinarization binMethod){

    //1) Is sample valid?
    if (isMatValid(originalMat)){

        //2) Is the sample larger enough?
        if (originalMat.cols > minDimension && originalMat.rows > minDimension) {

            //3) Is the sample too large?
            if (originalMat.cols < maxDimension && originalMat.rows < maxDimension) {

                //4) Can we creating a resized working mat?
                if(createWorkMat(desiredWidth, desiredHeight)){

                    //5) Can we create a grayscale mat from the working mat?
                    if(createGrayscaleMat()){

                        //6) Can we create a binary mat from the grayscale mat?
                        if(createBinaryMat(binMethod)){

                            //7) Can we create the XY Cut mats?
                            if(createXYCutMat()){

                               //TODO
                               // string folder = getFolderName(mFilename);
                               string folder =  "/home/gutto/Desktop/projects/samples/";

                                //Should we save the intermediate files?
                                //saveMat(workMat, folder + "/temp/work_" + getFileName(mFilename));
                                //saveMat(grayMat, folder + "/temp/gray_" + getFileName(mFilename));
                                saveMat(binaryMat, folder + "/temp/binary_" + getFileName(mFilename));
                                //saveMat(XYCutMat, folder + "/temp/xycut_" + getFileName(mFilename));

                                Log(log_Detail, "sample.cpp", "preProcess", "            Done. Sample was pre-processed successfully.");
                                return true;

                            }else
                                Log(log_Error, "sample.cpp", "preProcess", "            Ignoring sample because xyCut mat is invalid ('%s').", mFilename.c_str());
                        }else
                            Log(log_Error, "sample.cpp", "preProcess", "            Ignoring sample because binary mat is invalid ('%s').", mFilename.c_str());
                    }else
                        Log(log_Error, "sample.cpp", "preProcess", "            Ignoring sample because grayscale mat is invalid ('%s').", mFilename.c_str());
                }else
                    Log(log_Error, "sample.cpp", "preProcess", "            Ignoring sample because working mat is invalid ('%s').", mFilename.c_str());
            }else
                Log(log_Error, "sample.cpp", "preProcess", "            Ignoring sample because onde of its dimension is larger than %i pixels ('%s').", maxDimension, mFilename.c_str());
        }else
            Log(log_Error, "sample.cpp", "preProcess", "            Ignoring sample because one of its dimension is smaller than %i pixels ('%s').", minDimension, mFilename.c_str());
    }else
        Log(log_Error, "sample.cpp", "preProcess", "            Ignoring sample because original mat is invalid ('%s').", mFilename.c_str());

    return false;
}

bool Sample::saveMat(Mat inputMat, string filename){

    Log(log_Detail, "sample.cpp", "saveMat", "         Saving mat as '%s'...", filename.c_str());

    if(isMatValid(inputMat)) {

        //saves mat to file
        imwrite(filename, inputMat);

        Log(log_Detail, "sample.cpp", "saveMat", "            Done.");
        return true;

    }

    Log(log_Error, "sample.cpp", "saveMat", "            Saving mat failed.");
    return false;
}

bool Sample::createWorkMat(int width, int height){

    try{
        Log(log_Detail, "sample.cpp", "createWorkMat", "      Creating the work mat...");

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

    Log(log_Detail, "sample.cpp", "createGrayscaleMat", "         Creating grayscale mat...");

    if(isMatValid(workMat)){

        //convert the originalMat to grayscale (ignores it if is already grayscale). This functions combines RGB values with weights R=, G= and B=)
        cvtColor(workMat, grayMat, CV_BGR2GRAY);

        if(isMatValid(grayMat)){
            Log(log_Detail, "sample.cpp", "createGrayscaleMat", "            Done.");
            return true;
        }
    }

    Log(log_Warning, "sample.cpp", "createGrayscaleMat", "            Creating grayscale mat failed.");
    return false;
}

bool Sample::createBinaryMat(enumBinarization binMethod){

    Log(log_Detail, "sample.cpp", "createBinaryMat", "         Creating binary mat...");

    if(isMatValid(grayMat)) {

        //converts the gray mat to a black and white one
        binarize(grayMat, binaryMat, binMethod);

        if(isMatValid(binaryMat)){
            Log(log_Detail, "sample.cpp", "createBinaryMat", "            Done.");
            return true;
        }
    }

    Log(log_Warning, "sample.cpp", "createBinaryMat", "            Creating binary mat failed.");
    return false;
}

bool Sample::createXYCutMat(){

    Log(log_Detail, "sample.cpp", "createXYCutMat", "         Creating XYCut mats from binary mat...");

    if(isMatValid(binaryMat)) {

        getXYCut(binaryMat, XYCutMat);

        if(isMatValid(XYCutMat) ){
            Log(log_Detail, "sample.cpp", "createXYCutMat", "            Done..");
            return true;
        }
    }

    Log(log_Warning, "sample.cpp", "createXYCutMat", "            Creating XYCut mats failed.");
    return false;
}
