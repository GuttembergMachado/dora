//
// Guttemberg Machado on 24/07/17.
//
#include "sample.h"

Sample::Sample() {

    //Initialize the internal variables
    mLabel = "";
    mFilename = "";
    mTemporaryFolder = "";

};

bool Sample::load(string filename, string label, bool fixBrokenJPG) {

    try {
        Log(log_Detail, "sample.cpp", "load", "      Loading file '%s'...", filename.c_str());

        mFilename = filename;
        mLabel = label;

        //Loads an unchanged mat from the image file
        originalMat = imread(mFilename, CV_LOAD_IMAGE_COLOR);

        //Do we have a mat?
        if (isMatValid(originalMat)) {

            if (fixBrokenJPG) {
                Log(log_Detail, "sample.cpp", "load",
                    "      Saving file again to avoid the 'Premature end of JPEG file' exception...");
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

bool Sample::set(Mat inputMat){
    
    try {
        Log(log_Detail, "sample.cpp", "set", "      Loading mat...");
        
        mFilename = "";
        mLabel = "";
        
        //Loads an unchanged mat from the image file
        originalMat = inputMat;
        
        //Do we have a mat?
        if (isMatValid(originalMat)) {
            
            Log(log_Detail, "sample.cpp", "set", "      Mat was set.");
            return true;
            
        } else {
            Log(log_Error, "sample.cpp", "set", "      Failed to set mat.");
        }
        
    } catch (const std::exception &e) {
        Log(log_Error, "sample.cpp", "load", "         Error setting mat: %s", e.what());
    }
    
    return false;
    
}

string Sample::getLabel() {
    return mLabel;
}

string Sample::getFilename() {
    return mFilename;
}

void Sample::setTemporaryFolder(string folder) {
    mTemporaryFolder = folder;
}

bool Sample::preProcess(int desiredDimension, enumRescale rescaleMethod, enumBinarization binMethod) {

    try {
        //1) Is sample valid?
        if (isMatValid(originalMat)) {
            
            //2) Can we creating a resized working mat?
            if (createWorkMat(desiredDimension, rescaleMethod)) {

                //3) Can we create a grayscale mat from the working mat?
                if (createGrayscaleMat()) {

                    //4) Can we create a binary mat from the grayscale mat?
                    if (createBinaryMat(binMethod)) {

                        //5) Can we create the XY Cut mats?
                        if (createXYCutMat()) {
                            
                            if(mFilename != ""){
                                
                                string filename = getFileName(mFilename);
                                string extension = toLower(filename.substr(filename.find_last_of(".") + 1));
                                filename = toLower(filename.substr(0, filename.find_last_of(".") ));
    
                                //Should we save the intermediate files?
                                //saveMat(originalMat,  mTemporaryFolder + filename + "_original." + extension );
    
                                //if(rescaleMethod != rescale_NONE)
                                //    saveMat(workMat,      mTemporaryFolder + filename + "_work_" + mLabel + "." + extension);
    
                                //saveMat(grayMat,      mTemporaryFolder + filename + "_grayscale" + mLabel + "." + extension);
    
                                //if(binMethod != binarization_NONE)
                                //    saveMat(binaryMat,    mTemporaryFolder + filename + "_" +  mLabel + "." + extension);
                                saveMat(binaryMat,    mTemporaryFolder + mLabel + "_" + filename + "." + extension);
    
                                //saveMat(XYCutMat,     mTemporaryFolder + filename + "_xycut"     + mLabel + "." + extension);

                            }
    
                            Log(log_Detail, "sample.cpp", "preProcess","            Done. Sample was pre-processed successfully.");
                            return true;
                            
                        }else
                            Log(log_Error, "sample.cpp", "preProcess", "            Ignoring sample because xyCut mat is invalid ('%s').", mFilename.c_str());
                    }else
                        Log(log_Error, "sample.cpp", "preProcess","            Ignoring sample because binary mat is invalid ('%s').", mFilename.c_str());
                }else
                    Log(log_Error, "sample.cpp", "preProcess", "            Ignoring sample because grayscale mat is invalid ('%s').", mFilename.c_str());
            }else
                Log(log_Error, "sample.cpp", "preProcess", "            Ignoring sample because working mat is invalid ('%s').", mFilename.c_str());
        } else
            Log(log_Error, "sample.cpp", "preProcess","            Ignoring sample because original mat is invalid ('%s').", mFilename.c_str());
   
    } catch (const std::exception &e) {
        Log(log_Error, "sample.cpp", "createWorkMat", "         Failed to create mat: %s", e.what());
    }
    
    return false;
}

bool Sample::saveMat(Mat inputMat, string filename) {

    Log(log_Detail, "sample.cpp", "saveMat", "         Saving mat as '%s'...", filename.c_str());

    if (isMatValid(inputMat)) {
        
        //saves mat to file
        imwrite(filename, inputMat);

        Log(log_Detail, "sample.cpp", "saveMat", "            Done.");
        return true;

    }

    Log(log_Error, "sample.cpp", "saveMat", "            Saving mat failed.");
    return false;
}

bool Sample::createWorkMat(int desiredDimension, enumRescale rescaleMethod) {
    
    //To shrink an image, it will generally look best with CV_INTER_AREA interpolation,
    //To enlarge an image, it will generally look best with CV_INTER_CUBIC (slow) or CV_INTER_LINEAR (faster but still looks OK).

    try {
        Log(log_Detail, "sample.cpp", "createWorkMat", "      Creating work mat...");

        if (isMatValid(originalMat)) {
            
            if(rescaleMethod != rescale_NONE) {
                
                int resizeMethod = 0;
                Mat tempMat;
                int newSize = (originalMat.cols > originalMat.rows ? originalMat.rows : originalMat.cols);
    
                //Log(log_Detail, "sample.cpp", "createWorkMat", "            Original size is W:%i x H:%i", originalMat.cols, originalMat.rows);
                //Log(log_Detail, "sample.cpp", "createWorkMat", "            Rescaled size is W:%i x H:%i", newSize, newSize);
                //Log(log_Detail, "sample.cpp", "createWorkMat", "            Desired size is W:%i x H:%i", desiredDimension, desiredDimension);
    
                switch (rescaleMethod) {
                    case rescale_CROP: {
                        Rect roi;
                        roi.x = (originalMat.cols > originalMat.rows ? (originalMat.cols / 2) - (newSize / 2) : 0);
                        roi.y = (originalMat.cols > originalMat.rows ? 0 : (originalMat.rows / 2) - (newSize / 2));
                        roi.width = newSize;
                        roi.height = newSize;
                        tempMat = originalMat(roi);
                        break;
                    }
                    case rescale_SCALE: {
                        tempMat = originalMat;
                        break;
                    }
                    case rescale_FIT: {
                        Rect roi;
                        tempMat = Mat(newSize, newSize, originalMat.type(), Scalar(255, 255, 255));
            
                        if (originalMat.cols > originalMat.rows) {
                            roi.width = newSize;
                            roi.x = 0;
                            roi.height = (originalMat.rows * newSize) / originalMat.cols;
                            roi.y = (newSize / 2) - (roi.height / 2);
                        } else {
                            roi.height = newSize;
                            roi.y = 0;
                            roi.width = (originalMat.cols * newSize) / originalMat.rows;
                            roi.x = (newSize / 2) - (roi.width / 2);
                        }
                        resize(originalMat, tempMat(roi), roi.size());
                        break;
                    }
                }
    
                //Are we shrinking
                if (desiredDimension > tempMat.cols)
                    resizeMethod = CV_INTER_CUBIC;
                else
                    resizeMethod = CV_INTER_AREA;
    
                Size s = Size(desiredDimension, desiredDimension);
                resize(tempMat, workMat, s, 0, 0, resizeMethod);
                
            }else{
                workMat = originalMat;
            }
            
            Log(log_Detail, "sample.cpp", "createWorkMat","            Done. Work mat created (original size was W:%i x H:%i, new size is W:%i, H:%i).", originalMat.cols, originalMat.rows, workMat.cols, workMat.rows);

            if (isMatValid(workMat)) {
                return true;
            }
        }

    } catch (const std::exception &e) {
        Log(log_Error, "sample.cpp", "createWorkMat", "         Failed to create work mat: %s", e.what());
    }

    Log(log_Warning, "sample.cpp", "createWorkMat", "      Creating work mat failed.");
    return false;
}

bool Sample::createGrayscaleMat() {
    
    try {
        Log(log_Detail, "sample.cpp", "createGrayscaleMat", "         Creating grayscale mat...");
    
        if (isMatValid(workMat)) {
    
            //convert the originalMat to grayscale (ignores it if is already grayscale). This functions combines RGB values with weights R=, G= and B=)
            cvtColor(workMat, grayMat, CV_BGR2GRAY);
            
            if (isMatValid(grayMat)) {
                Log(log_Detail, "sample.cpp", "createGrayscaleMat", "            Done. Grayscale mat was created.");
                return true;
            }
        }
    
    } catch (const std::exception &e) {
        Log(log_Error, "sample.cpp", "createGrayscaleMat", "         Failed to create grayscale mat: %s", e.what());
    }

    Log(log_Warning, "sample.cpp", "createGrayscaleMat", "            Creating grayscale mat failed.");
    return false;
}

bool Sample::createBinaryMat(enumBinarization binMethod) {
    
    try {
        Log(log_Detail, "sample.cpp", "createBinaryMat", "         Creating binary mat...");
    
        if (isMatValid(grayMat)) {
    
            //converts the gray mat to a black and white one
            binarize(grayMat, binaryMat, binMethod);
    
            if (isMatValid(binaryMat)) {
                Log(log_Detail, "sample.cpp", "createBinaryMat", "            Done. Binary mat created.");
                return true;
            }
        }
    
    } catch (const std::exception &e) {
        Log(log_Error, "sample.cpp", "createBinaryMat", "         Failed to create binary mat: %s", e.what());
    }

    Log(log_Warning, "sample.cpp", "createBinaryMat", "            Creating binary mat failed.");
    return false;
}

bool Sample::createXYCutMat() {
    
    try {
        Log(log_Detail, "sample.cpp", "createXYCutMat", "         Creating XYCut mats from binary mat...");
    
        if (isMatValid(binaryMat)) {
    
            getXYCut(binaryMat, XYCutMat);
    
            if (isMatValid(XYCutMat)) {
                Log(log_Detail, "sample.cpp", "createXYCutMat", "            Done. XYCut mat created.");
                return true;
            }
        }
        
    } catch (const std::exception &e) {
        Log(log_Error, "sample.cpp", "createXYCutMat", "         Failed to create XYCut mat: %s", e.what());
    }
    
    Log(log_Warning, "sample.cpp", "createXYCutMat", "            Creating XYCut mats failed.");
    return false;
}
