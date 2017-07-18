#ifndef HELPER_H
#define HELPER_H

#include <stdarg.h>
#include <unistd.h>
#include <string.h>		//C Strings
#include <string>		//STL Strings
#include <iostream>
#include <cstdio>
#include <vector>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

using namespace std;

enum logMode{
	log_Debug=7,
	log_Warning=6,
	log_Error=4,
};

void Log(logMode mode, string moduleName, string procedureName, string information, ...);

bool isFolder(string path);
vector<string> listFiles(string folder);
vector<string> loadImages(string filename);

unsigned GetTickCount();
string getCurrentTimeStamp();
string getElapsedTime(unsigned fromTime);

#endif
