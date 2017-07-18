#include "helper.h"

using namespace std;

string log_filename = "log.txt";

void Log(logMode mode, string moduleName, string procedureName, string information, ...){

	char info[256];
	va_list args;
	va_start (args, information);
	vsprintf (info, information.c_str(), args);
	va_end (args);
	information = info;

	string buffer = "";

	//Formata o nome do module com 18 chars
	moduleName.append("                  ");
	moduleName = moduleName.substr(0, 18);

	//Formata o nome da procedure com 20 chars
	procedureName.append("                    ");
	procedureName = procedureName.substr(0, 20);

	//Monta o texto que será logado
	buffer = moduleName + " | " + procedureName + " | " + information + "\n";

	//Exibe a string no console
    printf("%s", buffer.c_str());

	//Concatena o horário no buffer
	buffer = string(getCurrentTimeStamp()) + string(" | ").c_str() + buffer.c_str();

	//Abre o arquivo para append
	_IO_FILE * fp;
	fp = fopen(log_filename.c_str(), "a+");
	if (NULL != fp){

		//Adiciona o texto no arquivo
		fprintf(fp, "%s", buffer.c_str());

		//Fecha o arquivo
		fclose(fp);
	}

}

bool isFolder(string path){

	struct stat s;
	Log(log_Debug, "helper.cpp", "isFolder", "   Checking path ...");

	try {
		if (stat(path.c_str(), &s) == 0)
			if (s.st_mode & S_IFDIR) {
				Log(log_Debug, "helper.cpp", "isFolder", "      Path '%s' is a folder.", path.c_str());
				return true;
			}

	}catch(exception e){
		Log(log_Error, "helper.cpp", "isFolder", "      Failed to check path: %s", string(e.what()) ) ;
	}

	return false;

}

vector<string> listFiles(string folder){

	Log(log_Debug, "helper.cpp", "listFiles", "   Listing files...");

	vector<string> res;

	try{

		DIR * path;
		struct dirent *ent;
		path = opendir(folder.c_str());
		while ((ent = readdir(path)) != NULL) {
			string filename = ent->d_name;
			string file = folder.c_str() + filename;
			if (filename[0] == '.')
				continue;

			struct stat s;
			if (stat(file.c_str(), &s) == 0)
				if (s.st_mode & S_IFDIR)
					continue;

			res.push_back(filename);
			Log(log_Debug, "helper.cpp", "listFiles", "         %s", file.c_str());

		}
		closedir(path);

		int fileCount = res.size();

		Log(log_Debug, "helper.cpp", "listFiles", "      %i files were found.", fileCount);

	}catch(exception e){
		Log(log_Error, "helper.cpp", "listFiles",  "      Failed to list files: %s", string(e.what()) ) ;
	}

	return res;

}

vector<string> loadImages(string filename){

	Log(log_Debug, "helper.cpp", "loadImage", "   Loading image(s) from file...");

	try{
		int imageCount = 0;


		Log(log_Debug, "helper.cpp", "loadImage", "      %i images were loaded.", imageCount );

	}catch(exception e){
		Log(log_Error, "helper.cpp", "loadImage","      Failed to load image(s): %s", string(e.what()) ) ;
	}

}

unsigned GetTickCount()
{

	//LINUX-----------------------------------------------------------------------------------------------------------------------------
    struct timeval tv;
    if(gettimeofday(&tv, NULL) != 0)
       return 0;

	return (tv.tv_sec * 1000) + (tv.tv_usec / 1000);

	//LINUX-----------------------------------------------------------------------------------------------------------------------------

}

string getElapsedTime(unsigned fromTime){

	//LINUX-----------------------------------------------------------------------------------------------------------------------------

	//WINDOWS-----------------------------------------------------------------------------------------------------------------------------

	//return GetTickCount() - fromTime;

	return 0;

};

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


