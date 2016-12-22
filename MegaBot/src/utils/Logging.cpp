#include "Logging.h"
#include "asprintf\asprintf.h"
#include <string>
#include <cstdio>
#include <iostream>
#include <BWAPI.h>
#include <boost\format.hpp>

using namespace std;
using namespace BWAPI;

Logging* Logging::getInstance() {
    static Logging* instance = new Logging();
    return instance;
}

Logging::Logging(){
	filename = Configuration::OUTPUT_DIR + "log_MegaBot-vs-" + _enemyName() + ".log";
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
