#pragma once
#include <zip.h>
#include "Err.h"

class ZipFile
{
private:
	bool isValidFile_ = false;
	zip_t* file_;

public:
	explicit ZipFile(const char* filename);
	~ZipFile();

	Err AddFile(const uint8_t* fileContent, size_t bufferSize, const char* filename) const;
	Err ReadFile(const char* filename, uint8_t* fileBuffer, int bufferSize) const;
	Err DeleteFile(const char* filename) const;

	Err AddDir(const char* dirPath) const;

	bool IsValid() const;
};
