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
    //TODO:  2) Finish porting Flan, SIFT , BOW
    //TODO:  3) Finish porting classifier mode
    //TODO:  4) Check all the initialization procedure. Most of the opencv2.x functions are used differently on opencv 3.x
    //TODO:  5) Check what is the optimized size of a dictionary (based on samples, number of labels, etc)
    //DONE:  6) Include average sample sizes per class and over all sampels;

    Model mod;

    Log(log_Error, "main.cpp", "main", "-------------------------------------------------------------------------------");
    Log(log_Error, "main.cpp", "main", "DORA: Document Analysis and Recognition");
    Log(log_Error, "main.cpp", "main", "Guttemberg Meirelles Machado - 2015");
    Log(log_Error, "main.cpp", "main", "-------------------------------------------------------------------------------");
    Log(log_Error, "main.cpp", "main", "Logging and displaying: %s information.", getLogMode().c_str());

    //Checks if the number of parameters is right
    if (argc == 4){

        string mode = argv[1];
        string inputPath = argv[2];
        string model = argv[3];

        //Set the basic parameters of the model
        mod.setClassifierType(model_BAG_OF_FEATURES);
        mod.setFeatureType(feature_SIFT);
        mod.setMatcherType(matcher_FLANN);
        mod.setBinarizationType(binarization_BRADLEY);
        mod.setFilename(model);

        //Is it the modeler  mode?
        if (mode == "-m" || mode == "-M"){

            Log(log_Debug, "main.cpp", "main", "Entering MODELLER mode:");

            if(mod.initialize())
                if(mod.create(inputPath))
                    mod.save();

            //Is it the testing mode?
        }else if (mode == "-c" || mode == "-C") {

            Log(log_Debug, "main.cpp", "main", "Entering CLASSIFIER mode:");
            Log(log_Debug, "main.cpp", "main", "   Input file or folder: '%s'", inputPath.c_str());
            Log(log_Debug, "main.cpp", "main", "   Model filename: '%s'", mod.getFilename().c_str());
            Log(log_Debug, "main.cpp", "main", "   ");

            vector<string> files;
            vector<string> images;

            //Is the input a folder?
            if (isFolder(inputPath))
                files = listFiles(inputPath);
            else
                files = loadImages(inputPath);

            //load all images from files (in case of multi-image file formats, such as pdf, tiff, etc...)
            for (int i = 0; i < files.size(); i++)
                images = loadImages(files[i]);

            if (images.size() >= 1){

                //loads the model
                if(mod.load()){

                    for (int i = 0; i < images.size(); i++){

                        //loads the image
                        //doc.loadFile(images[i]);

                        //classify the image
                        //post.postProcessDoc(doc);
                    }
                }
            }else{
                Log(log_Error, "main.cpp", "main", "   No supported images were found.");
            }

        //Is it the helper mode
        }else if (mode == "-h" || mode == "-H"){
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
    }else{
        Log(log_Error, "main.cpp", "main", "   Missing command line argument. Try 'dora --h' for more information.");
    }

    Log(log_Error, "main.cpp", "main", "-------------------------------------------------------------------------------");

    Log(log_Error, "main.cpp", "main", "Press any key to exit");
    getchar();

    //exit
    return 0;

}

