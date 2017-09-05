//
// Guttemberg Machado on 24/07/17.
//
#include <iostream>
#include "./tools/helper.h"
#include "./model/model.h"

using namespace std;

int main(int argc, char **argv){

    //TODO:  1) Play with com o Tesseract:
    //          #include <tesseract/baseapi.h>
    //          tesseract::TessBaseAPI ocr;
    //TODO:  2) Check what is the optimized size of a dictionary (based on samples, number of labels, etc)
    //TODO:  3) Create a loadImage to read multi image files (pdf, tiff, etc)...
    
    Model mod;
    int64 startTask = getTick();

    Log(log_Error, "main.cpp", "main", "-------------------------------------------------------------------------------");
    Log(log_Error, "main.cpp", "main", "DORA: Document Analysis and Recognition");
    Log(log_Error, "main.cpp", "main", "Guttemberg Meirelles Machado - 2015");
    Log(log_Error, "main.cpp", "main", "-------------------------------------------------------------------------------");
    Log(log_Error, "main.cpp", "main", "Log mode is:Logging and displaying: %s information.", getLogMode().c_str());
    Log(log_Error, "main.cpp", "main", "Logging and displaying: %s information.", getLogMode().c_str());

    string arg1 = (argc > 1 ? toLower(argv[1]) : "-h");
    string arg2 = (argc > 2 ? argv[2] : "");
    string arg3 = (argc > 3 ? argv[3] : "");
    string arg4 = (argc > 4 ? argv[4] : "");

    Log(log_Debug, "main.cpp", "main", "   argument 1: '%s'", arg1.c_str());
    Log(log_Debug, "main.cpp", "main", "   argument 2: '%s'", arg2.c_str());
    Log(log_Debug, "main.cpp", "main", "   argument 3: '%s'", arg3.c_str());
    Log(log_Debug, "main.cpp", "main", "   argument 4: '%s'", arg4.c_str());
    
    if (arg1 == "-m" || arg1 == "-c") {
        mod.setClassifierType(model_BAG_OF_FEATURES);
        mod.setFeatureType(feature_SIFT);
        mod.setMatcherType(matcher_FLANN);
        mod.setBinarizationType(binarization_WOLFJOLION);
        mod.setRescaleType(rescale_FIT);
    }
    
    //Is it the modeler mode?
    if (arg1 == "-m"){

        Log(log_Debug, "main.cpp", "main", "Entering MODELLER mode:");

        string inputPath = arg2;
        string modelFilename = arg3;
        string tempFolder = arg4;
        
        mod.setFilename(modelFilename);
        mod.setTempFolder(tempFolder);
        
        //Initialize model engine
        if(mod.initialize())

            //Creates a new model file
            if(mod.create(inputPath))

                //Saves the new created file
                mod.save();

    //Is it the testing mode?
    }else if (arg1 == "-c") {

        Log(log_Debug, "main.cpp", "main", "Entering CLASSIFIER mode:");
    
        string inputPath = arg2;
        string modelFilename = arg3;
        string tempFolder = arg4;
        
        mod.setTempFolder(tempFolder);
        mod.setFilename(modelFilename);

        //Initialize model engine
        if (mod.initialize())
    
            //Loads and existing model file
            if (mod.load())
        
                //Classifies the input path
                mod.test(inputPath);
        
    //Is it the debug mode
    }else if (arg1 == "-d"){

        Log(log_Debug, "main.cpp", "main", "Entering DEBUG mode:");
    
        string tempPath = arg2;
        string debugFolder = getCurrentFolder().c_str() + string("/samples/debug/");
        
        Sample s;
        s.setTemporaryFolder(tempPath);
        
        //---------------------------------- DEBUG ONLY ----------------------------------
        //TESTING THE RESCALE METHODS
        //--------------------------------------------------------------------------------
        s.load(debugFolder + "vertical.jpg", "rescale_CROP", false);
        s.preProcess(100,  rescale_CROP, binarization_NONE);
        
        s.load(debugFolder + "vertical.jpg", "rescale_SCALE", false);
        s.preProcess(100,  rescale_SCALE, binarization_NONE);
        
        s.load(debugFolder + "vertical.jpg", "rescale_FIT", false);
        s.preProcess(100,  rescale_FIT, binarization_NONE);
        
        s.load(debugFolder + "horizontal.jpg", "rescale_CROP", false);
        s.preProcess(100,  rescale_CROP, binarization_NONE);
        
        s.load(debugFolder + "horizontal.jpg", "rescale_SCALE", false);
        s.preProcess(100,  rescale_SCALE, binarization_NONE);
        
        s.load(debugFolder + "horizontal.jpg", "rescale_FIT", false);
        s.preProcess(100,  rescale_FIT, binarization_NONE);
        
        s.load(debugFolder + "square.jpg", "rescale_CROP", false);
        s.preProcess(100,  rescale_CROP, binarization_NONE);
        
        s.load(debugFolder + "square.jpg", "rescale_SCALE", false);
        s.preProcess(100,  rescale_SCALE, binarization_NONE);
        
        s.load(debugFolder + "square.jpg", "rescale_FIT", false);
        s.preProcess(100,  rescale_FIT, binarization_NONE);

        //---------------------------------- DEBUG ONLY ----------------------------------
        //TESTING THE BINARIZATION METHODS
        //--------------------------------------------------------------------------------
        s.load(debugFolder + "lena.bmp", "binarization_TRESHOLD", false);
        s.preProcess(100, rescale_NONE,   binarization_TRESHOLD);

        s.load(debugFolder + "lena.bmp", "binarization_MEAN", false);
        s.preProcess(100, rescale_NONE,   binarization_MEAN);

        s.load(debugFolder + "lena.bmp", "binarization_GAUSSIAN", false);
        s.preProcess(100, rescale_NONE,   binarization_GAUSSIAN);

        s.load(debugFolder + "lena.bmp", "binarization_NIBLACK", false);
        s.preProcess(100, rescale_NONE,   binarization_NIBLACK);

        s.load(debugFolder + "lena.bmp", "binarization_SAUVOLA", false);
        s.preProcess(100, rescale_NONE,   binarization_SAUVOLA);

        s.load(debugFolder + "lena.bmp", "binarization_WOLFJOLION", false);
        s.preProcess(100, rescale_NONE,   binarization_WOLFJOLION);

        s.load(debugFolder + "lena.bmp", "binarization_BRADLEY", false);
        s.preProcess(100, rescale_NONE,   binarization_BRADLEY);

        s.load(debugFolder + "lena.bmp", "binarization_CLAHE", false);
        s.preProcess(100, rescale_NONE,   binarization_CLAHE);
 
        s.load(debugFolder + "page.png", "binarization_TRESHOLD", false);
        s.preProcess(100, rescale_NONE,   binarization_TRESHOLD);
    
        s.load(debugFolder + "page.png", "binarization_MEAN", false);
        s.preProcess(100, rescale_NONE,   binarization_MEAN);
    
        s.load(debugFolder + "page.png", "binarization_GAUSSIAN", false);
        s.preProcess(100, rescale_NONE,   binarization_GAUSSIAN);
    
        s.load(debugFolder + "page.png", "binarization_NIBLACK", false);
        s.preProcess(100, rescale_NONE,   binarization_NIBLACK);
    
        s.load(debugFolder + "page.png", "binarization_SAUVOLA", false);
        s.preProcess(100, rescale_NONE,   binarization_SAUVOLA);
    
        s.load(debugFolder + "page.png", "binarization_WOLFJOLION", false);
        s.preProcess(100, rescale_NONE,   binarization_WOLFJOLION);
    
        s.load(debugFolder + "page.png", "binarization_BRADLEY", false);
        s.preProcess(100, rescale_NONE,   binarization_BRADLEY);
    
        s.load(debugFolder + "page.png", "binarization_CLAHE", false);
        s.preProcess(100, rescale_NONE,   binarization_CLAHE);

        s.load(debugFolder + "paragraph.jpg", "binarization_TRESHOLD", false);
        s.preProcess(100, rescale_NONE,   binarization_TRESHOLD);
    
        s.load(debugFolder + "paragraph.jpg", "binarization_MEAN", false);
        s.preProcess(100, rescale_NONE,   binarization_MEAN);
    
        s.load(debugFolder + "paragraph.jpg", "binarization_GAUSSIAN", false);
        s.preProcess(100, rescale_NONE,   binarization_GAUSSIAN);
    
        s.load(debugFolder + "paragraph.jpg", "binarization_NIBLACK", false);
        s.preProcess(100, rescale_NONE,   binarization_NIBLACK);
    
        s.load(debugFolder + "paragraph.jpg", "binarization_SAUVOLA", false);
        s.preProcess(100, rescale_NONE,   binarization_SAUVOLA);
    
        s.load(debugFolder + "paragraph.jpg", "binarization_WOLFJOLION", false);
        s.preProcess(100, rescale_NONE,   binarization_WOLFJOLION);
    
        s.load(debugFolder + "paragraph.jpg", "binarization_BRADLEY", false);
        s.preProcess(100, rescale_NONE,   binarization_BRADLEY);
    
        s.load(debugFolder + "paragraph.jpg", "binarization_CLAHE", false);
        s.preProcess(100, rescale_NONE,   binarization_CLAHE);

    // Is it the helper mode
    }else if (arg1 == "-h"){
        Log(log_Debug, "main.cpp", "main", "-------------------------------------------------------------------------------");
        Log(log_Debug, "main.cpp", "main", "Dora is a document classifier.");
        Log(log_Debug, "main.cpp", "main", "");
        Log(log_Debug, "main.cpp", "main", "Usage:");
        Log(log_Debug, "main.cpp", "main", "   dora <options> input model");
        Log(log_Debug, "main.cpp", "main", "");
        Log(log_Debug, "main.cpp", "main", "   options:");
        Log(log_Debug, "main.cpp", "main", "      -h      	Displays this information.");
        Log(log_Debug, "main.cpp", "main", "      -m      	Modeler Mode. Used to train a model based on a set of files.");
        Log(log_Debug, "main.cpp", "main", "      -c      	Classifier Mode; Used to classify documents.");
        Log(log_Debug, "main.cpp", "main", "      sample_folder	Folder with pre-classified images. Sub-folder name should be the label of the pre-classified images.");
        Log(log_Debug, "main.cpp", "main", "      document 	Document file or folder containing (jpg, png, bmp or pdf");
        Log(log_Debug, "main.cpp", "main", "      model_file  	Specify a model filename. It will be written in modeler mode, and read in classifier mode.");
        Log(log_Debug, "main.cpp", "main", "");
        Log(log_Debug, "main.cpp", "main", "   examples:");
        Log(log_Debug, "main.cpp", "main", "      dora -h");
        Log(log_Debug, "main.cpp", "main", "      dora -m 'c:/samples/' 'c:/docs/model.xml'");
        Log(log_Debug, "main.cpp", "main", "      dora -c 'c:/docs/doc.jpg' 'c:/docs/model.xml'");
        Log(log_Debug, "main.cpp", "main", "      dora -c 'c:/docs' 'c:/docs/model.xml'");
        Log(log_Debug, "main.cpp", "main", "      dora -c 'c:/docs/*.png' 'c:/docs/model.xml'");
    }else{
        Log(log_Error, "main.cpp", "main", "   Unknown command line argument. Try 'dora --h' for more information.");
    }

    Log(log_Error, "main.cpp", "main", "Finished after %s seconds.", getDiffString(startTask).c_str());
    Log(log_Error, "main.cpp", "main", "-------------------------------------------------------------------------------");

    Log(log_Error, "main.cpp", "main", "Press any key to exit");
    getchar();

    //exit
    return 0;

}

