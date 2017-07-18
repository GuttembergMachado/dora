
#ifndef DOCUMENT_H
#define DOCUMENT_H

#include <string>
#include <vector>
#include <time.h>
#include <windows.h>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace cv;
using namespace std;

class Document{
public:
	string	filename;
	string	type;
	long	filesize;
	Mat		originalMat;
	Mat		binaryMat;
	Mat		verticalProjectionMat;
	Mat		horizontalProjectionMat;
	Mat		projectionMat;
	void	loadFile(string fileName);
	void	saveBinaryFile(string fileName);
	void	saveProjectionFile(string fileName);
	void	dump();
};


#endif