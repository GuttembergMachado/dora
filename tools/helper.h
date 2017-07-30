//
// Guttemberg Machado on 24/07/17.
//
#ifndef HELPER_H
#define HELPER_H

#include <stdarg.h>
#include <unistd.h>
#include <string>
#include <string.h>
#include <iostream>
#include <cstdio>
#include <vector>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <vector>
#include <libgen.h>
#include <opencv2/core/core.hpp>

using namespace cv;
using namespace std;

enum logMode{
	log_Error=0,
	log_Warning=1,
	log_Debug=2,
	log_Detail =3
};

string getLogMode();
void Log(logMode mode, string moduleName, string procedureName, string information, ...);

bool isFolder(string path);
bool isFile(string path);
int  fileSize(string filename);

vector<string> listFiles(string folder);
vector<string> loadImages(string filename);
string getFileName(string path);
string getFolderName(string path);
string replace(string str, string from, string to);

int64 getTick();
double getDif(int64 startTick);
string getDifString(int64 startTick);

string getCurrentTimeStamp();
string getElapsedTime(unsigned fromTime);


#endif
