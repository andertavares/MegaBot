#include "FileCounter.h"
#include <string>
#include <windows.h>
#include <tchar.h> 
#include <stdio.h>
#include <strsafe.h>
#include <exception>
#include <iostream> 
#include "Logging.h"

using namespace std;

FileCounter::FileCounter(void){
}


FileCounter::~FileCounter(void){
}

int FileCounter::countFiles(string pattern){
	int fileCount = 0;
	
	//code to glob files - https://msdn.microsoft.com/en-us/library/windows/desktop/aa365200(v=vs.85).aspx
	WIN32_FIND_DATAA ffd;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	DWORD dwError = 0;
	
	//Logging::getInstance()->log("Looking for pattern  %s", pattern.c_str());	//DANGER: what if Logging instance has not been initialized?

	hFind = FindFirstFileA(pattern.c_str(), &ffd);

	//test error number for invalid_handle: 
	// 0 might be pattern not found; 2 is file not found, which are OK -> go create first file
	if (INVALID_HANDLE_VALUE == hFind) 	{
		
		dwError = GetLastError();
		if(dwError == ERROR_SUCCESS || dwError == ERROR_FILE_NOT_FOUND){
			//Logging::getInstance()->log("It seems that pattern was not found, you'll need to create the first file.");
			fileCount = -1; //adjusts fileCount because it's gonna be incremented below, even when there is no file
		}
		else {
			//this used to happen with ERRNO 183, because we were using a wrong function to look for files
			throw ios_base::failure("Error while looking for pattern. Check the log for more information");
		}
	} 
   
	// count files in the directory
	do {
		fileCount++;
	}
	while (FindNextFileA(hFind, &ffd) != 0);

	return fileCount;
}
