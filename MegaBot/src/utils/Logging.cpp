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
#include "FileCounter.h"

using namespace std;
using namespace BWAPI;

Logging* Logging::getInstance() {
    static Logging* instance = new Logging();
    return instance;
}

Logging::Logging() : logLevel(Logging::INFO){
	/* 
	 * Defines the log filename. Attempts to go for log_MegaBot-vs-enemy(X+1).log 
	 * where X is the number of last existing file (padded with up to 6 zeros to left).
	 * Defaults to log_default-MegaBot-vs-enemy.log if some problem happens
	 */

	//int logCount = 0;
	string pathPrefix =  Configuration::OUTPUT_DIR + "log_MegaBot-vs-" + _enemyName();
	filename = Configuration::OUTPUT_DIR + "log_default-MegaBot-vs-" + _enemyName() + ".log";	//default filename

	string writePattern = pathPrefix + "*.log";
	string readPattern = Configuration::READ_DIR + "log_MegaBot-vs-" + _enemyName() + "*.log";
	
	try {
		//since files may exist both in read and write dirs, look in both and gets the highest number
		int logCount = max(FileCounter::countFiles(writePattern), FileCounter::countFiles(readPattern));

		//pad with zeroes to the right up to 6 digits
		char fileNumber[7];
		sprintf_s(fileNumber, sizeof(fileNumber), "%06d", logCount + 1);	//new file's gonna have incremented digit
	
		log("Gonna write log to %s%s.log", pathPrefix.c_str(), fileNumber);
		filename = pathPrefix + string(fileNumber) + ".log";
	}
	catch (ios_base::failure e){
		//this used to happen with ERRNO 183, because we were using a wrong function to look for files
		log("An error occurred while looking for pre-existing log files... Logging to default file: %s", filename.c_str());
		logWindowsError(TEXT("FileCounter::countFiles"));
		return;
	}

	
	
}
/*
void Logging::setLogFileNumber(string prefix, int number) {
	//pad with zeroes to the right
	char fileNumber[5];
	sprintf_s(fileNumber, sizeof(fileNumber), "%06d", number + 1);	//new file's gonna have incremented digit
	
	log("Gonna write log to %s%s.log", prefix.c_str(), fileNumber);
	filename = prefix + string(fileNumber) + ".log";
}
*/

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

void Logging::logWindowsError(LPTSTR lpszFunction) { 
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
