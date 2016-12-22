#ifndef __LOGGING_H__
#define __LOGGING_H__

#include <stdarg.h>
#include <BWAPI.h>
#include <boost\format.hpp>
#include "../data/Configuration.h"

class Logging {
public:
    ~Logging() { }

    static Logging* getInstance();

    void log(const char * msg, ...);

protected:
    FILE* file_ptr;
    string filename;

    Logging();
};

#endif