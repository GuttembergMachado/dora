# DORA - Document Object Recognition and Analysis


**Dora** is a multi-class document classifier. It is a supervised machine learning trainer and classifier used to classify scanned images of documents, photos of documents, or video frames containing documents. 

**This version is functional (as in 2017-Aug-29). Two samples are provided:  A deck of cards and its suits. I still working on the tutorial **

There are several different techniques and algorithms implemented on the MSVC++ version, including my own version of a Projection Algorithm based on XYCut. 

This repo is the first try out of a linux port, and an effort to adapt the code, that was previously written for opencv 2.4, in order to  to take advantage of some of the new features and patterns offered by opencv 3.2 (Or the new deap learning techniques from the jsut released opencv 3.3)...

Dora is a console application and although there's no setup required, it does requires an opencv (3.2 or latter) installed.
   
Dora can be used in two distinct modes: **Modeler** and **Classifier**. 

## Modeler mode
 This is the part that actually learns from image samples (previously classified) and generates a model.

 **Modeler mode** requires a folder with several samples. In order to teach **dora** what a sample is, similar samples must be grouped within the same folder. (eg: all social security images stored in a filesystem folder, while all driver licenses are stored on a different folder). The folder is than consider a **label**. Folders can be nested, if desired. Usually large sets of samples produces better classification results, but it takes longer to teach/learn. After processing the samples, a  **model** is created and saved as a file. This model is later on used to classify images on Classifier mode.


## Classifier Mode
 This is the part that uses whatever was learned (by loading the model generated on the modeler mode) to classify a real image.

 The **Classifier mode** requires an input set, that can be a single image file, a multi-image file (such as tiff, pdfs, etc) or a folder with those files, and after processing it returns a LABEL for each image.   
    
    
    
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
```       

There are a few undocumented parameters used to choose the algorithms used, and also what should be saved as intermediate files. Hopefully I will document them soon  (as I make sure they all work when together).




### Algorithms
The following algorithms are implemented:

#### Trainer Algorithm: 
- **Bag of Words** (It's actually a Bag of Image Features) *(default)*
- Matrix Deviation (actually being tested)
     
#### Image Features: 
- **Scale Invariant Feature Transform** *(default)*
- XYCut Projection as features (actually being tested) 
- Speed Up Robust Features 
- Local Binary Patterns 
- Feature From Accelerated Segment Tests 
- Binary Robust Independent Elementary Features 
- Oriented Fast & Rotated Binary Robust Features 
- Good Features To Track Detector 
   
#### Matcher Algorithms:
- **Fast Library for Approximating Nearest Neighbors** *(default)*
- Brute Force (actually being tested)
   
#### Binarization Algorithm 
- **Derek Bradley's algorithm** *(default)*
- Regular Thresholding
- Mean Thresholding
- Weighted Sum Area Thresholding
- Nobuyuki Otsu's algorithm
- Wayne Niblack's algorithm
- Jaakko J. Sauvola's algorithm
- Christian Wolf's algorithm
- Contrast Limited Adaptive Histogram



### Licensing

Most of the algorithms used are actually provided by opencv, except by **Matrix Deviation** and **XYCut Projection as Feature**, in which I am working on and hopefully show good results. Most of the opencv's algorithms are open-sourced, but since a few of them are patented you check its licenses before using them.



### Sample images
- Decks of cards (courtesy of http://freeware.esoterica.free.fr/html/freecards.html)

