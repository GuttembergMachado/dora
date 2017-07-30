# DORA - Document Object Recognition and Analysis


**Dora** is a document classifier. It is a supervised machine learning trainer and classifier used to classify scanned document images, document pictures and video frames of documents. There is serveral different techniques and algorithms implemented on its VC++ version. This repo is the first try out of a linux port.

Dora is a console application and there's no setup required, but it requires opencv.
    
    
    
```
Usage:
    dora <options> input model
 
    options:
       -h      	Displays this information.
       -m      	Modeler Mode. Used to train a model based on a set of files.
       -c      	Classifier Mode; Used to classify documents.
       sample_folder	Folder with pre-classified images. Sub-folder name should be the label of the pre-classified images.
       document 	Document file or folder containing (jpg, png, bmp or pdf
       model_file  	Specify a model filename. It will be written in modeler mode, and read in classifier mode.
 
    examples:
       dora -h
       dora -m 'c:/samples/' 'c:/docs/model.xml'
       dora -c 'c:/docs/doc.jpg' 'c:/docs/model.xml'
       dora -c 'c:/docs' 'c:/docs/model.xml'
       dora -c 'c:/docs/*.png' 'c:/docs/model.xml'


Sample images or models are not provided
    
    