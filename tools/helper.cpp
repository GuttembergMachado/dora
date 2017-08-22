//
// Guttemberg Machado on 24/07/17.
//
// This file not only has helper functinos by also tries to implement OS functions.
// After porting it to linux, these should be the only module of the project that
// we will need to change to port to another OS. In the feature we should check
// if 'compiler variables' are standart to all platforms so we can actually use them.
//

//TODO: Check out if compiler variables would work on both windows and linux
//TODO: Improve logging and the log levels
//

#include <list>
#include "helper.h"

using namespace std;

//	0; errors only
//	1; errors and warnings
//	2; errors, warnings and debug
//	3; errors, warnings, debug and details
int log_level = 2;

string log_filename = "log.txt";

string getLogMode(){
	switch (log_level){
		case 0:	return "errors only";
		case 1: return "errors and warnings";
		case 2: return "errors, warnings and debug";
		case 3:	return "errors, warnings, debug and details (might affect performance)";
	}
}

void Log(logMode mode, string moduleName, string procedureName, string information, ...){

	char info[256];
	va_list args;
	va_start (args, information);
	vsprintf (info, information.c_str(), args);
	va_end (args);
	information = info;

	if(log_level >= mode) {

		string buffer = "";

		//Formata o nome do module com 18 chars
		moduleName.append("                  ");
		moduleName = moduleName.substr(0, 18);

		//Formata o nome da procedure com 20 chars
		procedureName.append("                    ");
		procedureName = procedureName.substr(0, 22);

		//Monta o texto que será logado
		buffer = moduleName + " | " + procedureName + " | ";

		buffer += information + "\n";

		//Exibe a string no console
		printf("%s", buffer.c_str());

		//Concatena o horário no buffer
		buffer = string(getCurrentTimeStamp()) + string(" | ").c_str() + buffer.c_str();

		//Abre o arquivo para append
		_IO_FILE *fp;
		fp = fopen(log_filename.c_str(), "a+");
		if (NULL != fp) {

			//Adiciona o texto no arquivo
			fprintf(fp, "%s", buffer.c_str());

			//Fecha o arquivo
			fclose(fp);
		}
	}
}

bool isFile(string path){

	struct stat s;
	Log(log_Detail, "helper.cpp", "isFile", "   Checking path '%s'...", path.c_str());

	try {
		if (stat(path.c_str(), &s) == 0)
			if(S_ISREG(s.st_mode)){
				Log(log_Detail, "helper.cpp", "isFile", "      Path is an existing file.", path.c_str());
				return true;
			}
	}catch(const std::exception& e){
		Log(log_Error, "helper.cpp", "isFile", "      Failed to check path: %s", e.what() ) ;
	}

	return false;

}

int fileSize(string filename){

    struct stat s;

    try {
        if (stat(filename.c_str(), &s) == 0)
            return (intmax_t) s.st_size;

    }catch(const std::exception& e){
        Log(log_Error, "helper.cpp", "fileSize", "      Failed to get file size of %s", e.what() ) ;
    }

   return 0;


}

bool isFolder(string path){

	struct stat s;
	Log(log_Detail, "helper.cpp", "isFolder", "   Checking path '%s'...", path.c_str());

	try {
		if (stat(path.c_str(), &s) == 0)
			if (s.st_mode & S_IFDIR) {
				Log(log_Detail, "helper.cpp", "isFolder", "      Done. Path is an existing folder.", path.c_str());
				return true;
			}

	}catch(const std::exception& e){
		Log(log_Error, "helper.cpp", "isFolder", "      Failed to check path: %s", e.what() ) ;
	}

	return false;

}

string toLower(string input){

	string res = input;

	transform(res.begin(), res.end(), res.begin(), ::tolower);

	return res;

}

string toUpper(string input){

	string res = input;

	transform(res.begin(), res.end(), res.begin(), ::toupper);

	return res;

}

string getFileName(string path){

	char * s;

	try {

		char * f = strdup(path.c_str());
		s = basename(f);

	}catch(const std::exception& e){
		Log(log_Error, "helper.cpp", "getFileName", "      Failed to get file name from path: %s", e.what() ) ;
	}

	return s;

}

string getFolderName(string path){

	char * s;

	try {

		char * d = strdup(path.c_str());
		s = dirname(d);

	}catch(const std::exception& e){
		Log(log_Error, "helper.cpp", "getFolderName", "      Failed to get folder name from path: %s", e.what() ) ;
	}

	return s;
}

string replace(string str, string from, string to){

	const char * s;

	try {

        if(!from.empty()){}

            size_t start_pos = 0;
            while((start_pos = str.find(from, start_pos)) != std::string::npos) {
                str.replace(start_pos, from.length(), to);
                start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
            }

        s = str.c_str();

	}catch(const std::exception& e){
		Log(log_Error, "helper.cpp", "getFolderName", "      Failed to replace string: %s", e.what() ) ;
	}

	return s;
}

void addFilesToList(vector<string> & list, string startPath){

    struct dirent * ent;

    DIR * item = opendir(startPath.c_str());

    while ((ent = readdir(item)) != NULL) {

		string name = ent->d_name;
		string fullname = startPath.c_str() + name;

        struct stat s;

        if (stat(fullname.c_str(), &s) == 0){
			if(name[0] !='.'){
				if (s.st_mode & S_IFDIR) {
					addFilesToList(list, fullname + "/");
				}else{
					list.push_back(fullname);
				}
	        }
        }
    }

    closedir(item);

}

vector<string> listFiles(string folder){

	Log(log_Detail, "helper.cpp", "listFiles", "   Listing files from folder '%s'...", folder.c_str());

	vector<string> res;

	try{
        addFilesToList(res, folder);

		Log(log_Detail, "helper.cpp", "listFiles", "      Done. Found %i files.", res.size());

	}catch(const std::exception& e){
		Log(log_Error, "helper.cpp", "listFiles",  "      Error listing files: %s", e.what() ) ;
	}

	return res;

}

bool isMatValid(Mat m){

    bool bRet = (m.rows > 0 && m.cols > 0 && m.channels() > 0 && !m.empty());
    return bRet;
}

string getMatType(Mat m){
	switch (m.type()){
		case CV_8U:   return "CV_8U";
		case CV_8S:   return "CV_8S";
		case CV_16U:  return "CV_16U";
		case CV_16S:  return "CV_16S";
		case CV_32S:  return "CV_32S";
		case CV_32F:  return "CV_32F";
		case CV_64F:  return "CV_64F";
		default:      return "UNKNOWN";
	}
}

//---------------------------------------------------------------------------------------------------------------------------
int64 getTick(){
	return getTickCount();
}
double getDiff(int64 startTick){
	return (double) (getTick() - startTick) / getTickFrequency();
}
string getDiffString(int64 startTick){

	double d = getDiff(startTick);
	string s(16, '\0');
	auto w = snprintf(&s[0], s.size(), "%.2f", d);
	s.resize(w);
	return s;

}

string getCurrentTimeStamp(){

	//LINUX-----------------------------------------------------------------------------------------------------------------------------
    char buf[20];
    struct tm *sTm;

    time_t now = time(0);
    sTm = gmtime(&now);

    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", sTm);

    //WINDOWS-----------------------------------------------------------------------------------------------------------------------------
    //char buf[32];  //yyyy-mm-dd hh:nn:ss:mmm
    //SYSTEMTIME   sTm;
    //
	//GetLocalTime(&sTm);
	//
	//sprintf_s(buf, "%04d-%02d-%02d %02d:%02d:%02d:%03d", sTm.wYear, sTm.wMonth, sTm.wDay, sTm.wHour, sTm.wMinute, sTm.wSecond, sTm.wMilliseconds);

    return buf;
}


