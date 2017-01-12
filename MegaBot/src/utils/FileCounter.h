#pragma once
#include <string>
#include <windows.h>

/** 
  * Utility class to count files following a pattern 
  */
class FileCounter {
	

	/** Should not be instantiated */
	FileCounter(void);

public:
	/** Return how many files exist with given pattern (tested with prefix*.extension) */
	static int countFiles(std::string pattern);

	~FileCounter(void);
};

