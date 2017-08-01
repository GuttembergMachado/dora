//
// Guttemberg Machado on 31/07/17.
//

//TODO: Check both xCut and yCut cause they are not working properly

#include "xycut.h"

bool getXCut(Mat &source, Mat &dest){

	try{
		Mat1i tmpMat;
		reduce(source, tmpMat, 1, CV_REDUCE_SUM);//1=Colapse to a Single ROW
		tmpMat.copyTo(dest);
		return true;

	}catch(const std::exception& e){
		Log(log_Error, "xycut.cpp", "getXCut",  "         Failed to create xCut mat: %s", e.what() ) ;
	}

	return false;

}

bool getYCut(Mat &source, Mat &dest){

	try{
		Mat1i tmpMat;
		reduce(source, tmpMat, 0, CV_REDUCE_SUM); //0= Colapse to a single COLUMN
		tmpMat.copyTo(dest);

		return true;

	}catch(const std::exception& e){
		Log(log_Error, "xycut.cpp", "getXCut",  "         Failed to create yCut mat: %s", e.what() ) ;
	}

	return false;
}

bool getXYCut(Mat &source, Mat &dest){

	//========================================================================//
	//                                                                        //
	//   +--------+----+--------+                                             //
	//   |        |    |        |    where:                                    //
	//   |        |    |        |            I  = Imagem                      //
	//   |    I   | VP |   VR   |            VP = Vertical Projection         //
	//   |        |    |        |            VR = Veritical Representation    //
	//   |        |    |        |            HP = Horizontal Projection       //
	//   +--------+----+--------+            HR = Horizontal Representation   //
	//   +   HP   +                                                           //
	//   +--------+                                                           //
	//   |        |                                                           //
	//   |        |                                                           //
	//   |   HR   |                                                           //
	//   |        |                                                           //
	//   |        |                                                           //
	//   +--------+                                                           //
	//                                                                        //
	//========================================================================//

	try{
		//Cria o Mat
		Mat1i xMat;
		Mat1i yMat;

		reduce(source, xMat, 0, CV_REDUCE_SUM); //0 = Colapse to a single COLUMN
		reduce(source, yMat, 1, CV_REDUCE_SUM); //1 = Colapse to a single ROW

		Mat resultMat = Mat(source.rows * 2, source.cols * 2, CV_8U, Scalar::all(255));

		//We can normalize the result, in order to keey the values in a range using
		//	normalize(xMat, xMat, 0, inputMat.cols , NORM_MINMAX, -1, Mat());
		//	normalize(yMat, yMat, 0, inputMat.rows , NORM_MINMAX, -1, Mat());
		// but since the binary image already uses 0 to 255 it is easier to just divide by 255
		xMat = xMat / 255;
		yMat = yMat / 255;

		//Draws the x representation to the right side
		for (int y = 0; y < xMat.rows; y++)
			line(resultMat, Point(source.cols, y), Point(source.cols + (source.cols - xMat.at<int>(y)), y), Scalar(0, 0, 0), 1, 8, 0);

		//Draws the y representation to the lower side
		for (int x = 0; x < yMat.cols; x++)
			line(resultMat, Point(x, source.rows), Point(x, source.rows + (source.rows - yMat.at<int>(x))), Scalar(0, 0, 0), 1, 8, 0);

		//Draws the original image
		source.copyTo(resultMat(Rect(0, 0, source.cols, source.rows)));

		//Draw an edge
		rectangle(resultMat, Point(0, 0), Point(source.cols, source.rows), Scalar(0, 0, 0), 1, 8, 0);
		rectangle(resultMat, Point(1, 1), Point(source.cols - 1, source.rows - 1), Scalar(255, 255, 255), 1, 8, 0);
		rectangle(resultMat, Point(2, 2), Point(source.cols - 2, source.rows - 2), Scalar(0, 0, 0), 1, 8, 0);

		//Draws to the destination mat
		resultMat.copyTo(dest);

		return true;

	}catch(const std::exception& e){
		Log(log_Error, "xycut.cpp", "getXCut",  "         Failed to create XYCCut mat: %s", e.what() ) ;
	}

	return false;
}
