#include "Logging.h"
#include "asprintf\asprintf.h"
#include <string>
#include <cstdio>
#include <iostream>
#include <BWAPI.h>
#include <windows.h>
#include <tchar.h> 
#include <stdio.h>
#include <strsafe.h>

using namespace std;
using namespace BWAPI;

Logging* Logging::getInstance() {
    static Logging* instance = new Logging();
    return instance;
}

Logging::Logging(){
	/* 
	 * Defines the log filename. Attempts to go for log_MegaBot-vs-enemy(X+1).log 
	 * where X is the number of last existing file (padded with up to 6 zeros to left).
	 * Defaults to log_default-MegaBot-vs-enemy.log if some problem happens
	 */

	int logCount = 0;
	string pathPrefix =  Configuration::OUTPUT_DIR + "log_MegaBot-vs-" + _enemyName();
	filename = Configuration::OUTPUT_DIR + "log_default-MegaBot-vs-" + _enemyName() + ".log";	//default filename

	//code to glob files - https://msdn.microsoft.com/en-us/library/windows/desktop/aa365200(v=vs.85).aspx
	WIN32_FIND_DATAA ffd;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	DWORD dwError = 0;
	string pattern = pathPrefix + "*.log";

	//StringCchCopy(szDir, MAX_PATH, (TCHAR*) pathPrefix.c_str());
	//StringCchCopy(szDir, MAX_PATH, (TCHAR*) "log_MegaBot");
	//StringCchCat(szDir, MAX_PATH, TEXT("*.log"));
	log("Looking for pattern  %s", pattern.c_str());

	hFind = FindFirstFileA(pattern.c_str(), &ffd);

	//test error number for invalid_handle: 
	// 0 might be pattern not found; 2 is file not found, which are OK -> go create first file
	if (INVALID_HANDLE_VALUE == hFind) 	{
		
		dwError = GetLastError();
		if(dwError == ERROR_SUCCESS || dwError == ERROR_FILE_NOT_FOUND){
			log("It seems that pattern was not found, will create first file.");
			logCount = -1; //adjusts logCount because it's gonna be incremented below, even when there is no file
		}
		else {
			log("Error while looking for pattern %s. ERRNO: %d", pattern.c_str(), dwError);
			logWinError(TEXT("FindFirstFileA"));
			log("Logging to default file: %s", filename.c_str());
			return;
		}
	} 
   
	
	// count files in the directory
	do {
		logCount++;
	}
	while (FindNextFileA(hFind, &ffd) != 0);

	//pad with zeroes to the right up to 6 digits
	char fileNumber[7];
	sprintf_s(fileNumber, sizeof(fileNumber), "%06d", logCount + 1);	//new file's gonna have incremented digit
	
	log("Gonna write log to %s%s.log", pathPrefix.c_str(), fileNumber);
	filename = pathPrefix + string(fileNumber) + ".log";
	
}

void Logging::setLogFileNumber(string prefix, int number) {
	//pad with zeroes to the right
	char fileNumber[5];
	sprintf_s(fileNumber, sizeof(fileNumber), "%06d", number + 1);	//new file's gonna have incremented digit
	
	log("Gonna write log to %s%s.log", prefix.c_str(), fileNumber);
	filename = prefix + string(fileNumber) + ".log";
}


void Logging::log(const char * msg, ...) {
    file_ptr = fopen(filename.c_str(), "a");
	char* formatted;
		
    va_list args;
    va_start(args, msg);
	int result = vasprintf(&formatted, msg, args); //credits: http://asprintf.insanecoding.org/
	va_end(args);

	Broodwar->printf(formatted);
	fprintf(file_ptr, "[%d] %s%s", Broodwar->getFrameCount(), formatted, "\n");    
    fclose(file_ptr);

	free(formatted);
}

void Logging::logWinError(LPTSTR lpszFunction) { 
    // Retrieve the system error message for the last-error code

    LPVOID lpMsgBuf;
    LPVOID lpDisplayBuf;
    DWORD dw = GetLastError(); 

    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | 
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        dw,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR) &lpMsgBuf,
        0, NULL 
	);

    // Display the error message and clean up
    lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT, 
        (lstrlen((LPCTSTR)lpMsgBuf)+lstrlen((LPCTSTR)lpszFunction)+40)*sizeof(TCHAR)
	); 

    StringCchPrintf((LPTSTR)lpDisplayBuf, 
        LocalSize(lpDisplayBuf) / sizeof(TCHAR),
        TEXT("%s failed with error %d: %s"), 
        lpszFunction, dw, lpMsgBuf
	); 
    //MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK); 
	log("%s", (LPCTSTR)lpDisplayBuf); 

    LocalFree(lpMsgBuf);
    LocalFree(lpDisplayBuf);
}
