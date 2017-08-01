# DORA - Document Object Recognition and Analysis


**Dora** is a document classifier. It is a supervised machine learning trainer and classifier used to classify scanned images of documents, photos of documents, or video frames containing documents. 

There is serveral different techniques and algorithms implemented on its VC++ version, including my own version of a Projection Algorithm based on XYCut. 

This repo is the first try out of a linux port, as well as an effort to adapt the code, that was previously written for opencv 2.4, in order to  to take adavantage of the new features and patters offered from opencv 3.2.

Dora is a console application and although there's no setup required, it does requires an opencv 3.2 (or latter) installed.
   
Dora can be used in two distinct modes: **Modeler** and **Classifier**. 

##Modeler mode
 This is the part that actually learns from image samples previously classified.

 **Modeler mode** requires a folder with several samples. In order to "teach" dora what a sample is, similar samples must be within the same folder. (eg: all social security images must be inside a folder, while all drivers licenses must be on a different folder). Each folder correspond to a **label**. Usually large sets of samples produces better classification, but it takes longer to learn. After processing the samples, a  **model** is created and saved as a file. This model is used later on used to classify images on Classifier mode.

##Classifier Mode
 This is the part that uses whatever was learned to classify a real image.

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
- Speeded Up Robust Features 
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
- Regular Tresholding
- Mean Tresholding
- Weighted Sum Area Thresholding
- Nobuyuki Otsu's algorithm
- Wayne Niblack's algorithm
- Jaakko J. Sauvola's algorithm
- Christian Wolf's algorithm
- Contrast Limited Adaptive Histogram


### Licensing

Most of the algorithms used are actually provided by opencv, except by **Matrix Deviation** and **XYCut Projection as Feature**, in which I am working on and hopefully show good results. Most of the opencv's algorithms are open-sourced, but since a few of them are patented, check the licenses before using them.


### Sample images
- Deck of cards (courtesy of) https://code.google.com/archive/p/vector-playing-cards/
- 