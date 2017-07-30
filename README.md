 
    **Dora** is a document classifier. It is a supervised machine learning that uses opencv combined with other techniques. Written originally in VC++. This repo is a non-functional linux port.
    
    It is a console application and there's no setup required, but OpenCV should be installed.
    
 Usage:
   options:
      -h      	Displays this information.
      -m      	Modeler Mode. Used to train a model.
      -c      	Classifier Mode; Used to classify a document.
      sample_folder	Folder with pre-classified images. Sub-folder name should be the label of the pre-classified images.
      document 	Document file or folder containing (jpg, png, bmp or pdf)
      model_file  	Specify a model filename. It will be written in modeler mode, and read in classifier mode.

   examples:
      dora -h
      dora -m 'c:/samples/' 'c:/docs/model.xml'
      dora -c 'c:/docs/doc.jpg' 'c:/docs/model.xml'
