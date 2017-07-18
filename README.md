 
 Dora is a document classifier. It can be used create classfiers from a few sample data. It can also be used to classify e benchmark sucess and error ratio.  It is based on semi-spervisioned; It is based on open computer vision (opencv) and collects a fiew other techniques. Written originally in VC++ is being recently ported to linux;
 
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
