This folder contains sets that can be used to train, test and classify similar documents.

Each sample set is composed by:
   1) the **training** folder:  This is the folder where all training images should be located. If you create a subfolder, dora will assume the folder name is the label that categorizes the images inside them.
   2) the **testing** folder: This is the folder where a set of images (usually images that were not part of the trainning) will be classifier and the sucess rate will be calculated. Again, if you create a subfolder, dora will assume the folder name is the label that categorizes the images inside them and use that label to check if the reconition was right or wrong ant then compute the sucess rate
   3) the ** temp ** folder:  This is where all temporary images (Such as original, gray, black and white, projections, etc) are stored. Images on this folder are automatically created during the training or recognition process.

All samples provided here are collected from several internet sources.

   cards/suites: A set of XXX samples containing images of several cards suit. They are classified as: diamongs, heart, clubs or spades.
   
   cards/deck: A set of XXX samples containing images of several cards from several suits and different decks. They are classified as suits (diamongs, heart, clubs or spades) and the card number (A, 2, 3, 4, 5, 6, 7, 8, 9, J, Q, K and Joker)
   
   If you would like to contribute with a sample, I suggest that:
      100% of the image samples are classified manually (or at least checked)
      80% of the images should be on the training folder
      20& of the images should be on the testing folder.
      
   That way, we can correctly check the sucess rate of each model.      
   
