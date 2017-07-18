#include <iostream>
#include "./tools/helper.h"
#include "./model/model.h"

using namespace std;

int main(int argc, char **argv){

    Model 		mod;  	//O modelo de documento
    //Document 	doc;	//O documento de entrada
    //PreProcessor pre; //O motor de Pre Processamento (Binarização, Deskewing, Noise Reduction, etc)

    Log(log_Debug, "main.cpp", "main", "-------------------------------------------------------------------------------");
    Log(log_Debug, "main.cpp", "main", "DORA: Document Analysis and Recognition");
    Log(log_Debug, "main.cpp", "main", "Guttemberg Meirelles Machado - 2015");
    Log(log_Debug, "main.cpp", "main", "-------------------------------------------------------------------------------");

    //Verifica se foram passados os parâmetros
    if (argc == 4){

        string mode = argv[1];

        //Anota os parâmetros passados
        string inputArg = argv[2];
        string outputArg = argv[3];

        mod.classificationModel = model_BAG_OF_FEATURES;
        mod.featureType  = feature_SIFT;
        mod.matchingModel = matching_K_MEANS_CLUSTERING;

        //Verifica se é para usar no modo MODELER
        if (mode == "-m" || mode == "-M"){

            Log(log_Debug, "main.cpp", "main", "Entering MODELLER mode:");
            Log(log_Debug, "main.cpp", "main", "   Sample folder: '%s'", inputArg.c_str());
            Log(log_Debug, "main.cpp", "main", "   Model filename: '%s'", outputArg.c_str());

            vector<string> files;

            //Verifica se é uma pasta
            if(isFolder(inputArg)){

                //Carrega os arquivos da pasta
                files = listFiles(inputArg);

                //Cria um modelo de reconhecimento
                //if (mod.createModel(files)){

                    //Salva o modelo
                 //   mod.saveModel(outputArg);

                //}
            }else{
                Log(log_Error, "main.cpp", "main", "ERROR:  Sample folder should be a folder, not a file!");
            }

        }else if (mode == "-c" || mode == "-C") {

            Log(log_Debug, "main.cpp", "main", "Entering CLASSIFIER mode:");
            Log(log_Debug, "main.cpp", "main", "   Input file or folder: '%s'", inputArg.c_str());
            Log(log_Debug, "main.cpp", "main", "   Model filename: '%s'", outputArg.c_str());

            vector<string> files;
            vector<string> images;

            //Verifica se é uma pasta
            if (isFolder(inputArg))
                files = listFiles(inputArg);
            else
                files = loadImages(inputArg);

            //Carrega as imagens do(s) arquivo(s)
            for (int i = 0; i < files.size(); i++)
                images = loadImages(files[i]);

            if (images.size() >= 1){

                //Carrega o modelo de reconhecimento
                if(mod.loadModel()){

                    for (int i = 0; i < images.size(); i++){

                        //Carrega o documento
                        //doc.load(images[i]);

                        //Faz o post processamento
                        //post.postProcessDoc(doc);
                    }
                }
            }else{
                Log(log_Error, "main.cpp", "main", "ERROR: No supported images were found.");
            }
        }else if (mode == "-h" || mode == "-H"){
            Log(log_Debug, "main.cpp", "main", "-------------------------------------------------------------------------------");
            Log(log_Debug, "main.cpp", "main", "Dora is a document classifier that can be trained.");
            Log(log_Debug, "main.cpp", "main", "");
            Log(log_Debug, "main.cpp", "main", "Usage:");
            Log(log_Debug, "main.cpp", "main", "   dora <options> input model");
            Log(log_Debug, "main.cpp", "main", "");
            Log(log_Debug, "main.cpp", "main", "   options:");
            Log(log_Debug, "main.cpp", "main", "      -h      	Displays this information.");
            Log(log_Debug, "main.cpp", "main", "      -m      	Modeler Mode. Used to train a model.");
            Log(log_Debug, "main.cpp", "main", "      -c      	Classifier Mode; Used to classify a document.");
            Log(log_Debug, "main.cpp", "main", "      sample_folder	Folder with pre-classified images. Sub-folder name should be the label of the pre-classified images.");
            Log(log_Debug, "main.cpp", "main", "      document 	Document file or folder containing (jpg, png, bmp or pdf");
            Log(log_Debug, "main.cpp", "main", "      model_file  	Specify a model filename. It will be written in modeler mode, and read in classifier mode.");
            Log(log_Debug, "main.cpp", "main", "");
            Log(log_Debug, "main.cpp", "main", "   examples:");
            Log(log_Debug, "main.cpp", "main", "      dora -h");
            Log(log_Debug, "main.cpp", "main", "      dora -m 'c:/samples/' 'c:/docs/model.xml'");
            Log(log_Debug, "main.cpp", "main", "      dora -c 'c:/docs/doc.jpg' 'c:/docs/model.xml'");
        }else{
            Log(log_Error, "main.cpp", "main", "ERROR: Unknown command line argument. Try 'dora --h' for more information.");
        }
    }else{
        Log(log_Error, "main.cpp", "main", "ERROR:  Missing command line argument. Try 'dora --h' for more information.");
    }

    Log(log_Debug, "main.cpp", "main", "-------------------------------------------------------------------------------");

    //Encerra o programa
    return 0;

}

