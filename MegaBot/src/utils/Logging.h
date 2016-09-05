#ifndef __LOGGING_H__
#define __LOGGING_H__

#include <BWAPI.h>
#include "../data/Configuration.h"

class Logging {
public:
    ~Logging() {

    }
    static Logging* getInstance() {
        static Logging* instance = new Logging();
        return instance;
    }

    void log(const char * msg, ...) {
        file_ptr = fopen(filename.c_str(), "a");
        va_list args;
        va_start(args, msg);
        vfprintf(file_ptr, msg, args);
        fprintf(file_ptr, "\n");
        va_end(args);
        fclose(file_ptr);
    }

protected:
    FILE* file_ptr;
    string filename;

    Logging() {
        filename = "Log_MegaBot-vs-" + _enemyName() + ".txt";
    }
};


#endif