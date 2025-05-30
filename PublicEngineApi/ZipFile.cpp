#include "ZipFile.h"
#include "SystemPathHelper.h"

#undef DeleteFile

ZipFile::ZipFile(const char* filename)
{
	int errCode;
	std::string strFilename = filename;

	if ((file_ = zip_open(SystemPathHelper::RemoveRelativeSlash(strFilename).c_str(), ZIP_CREATE, &errCode)) == nullptr)
	{
		zip_error_t error;
		zip_error_init_with_code(&error, errCode);

		return;
	}

	isValidFile_ = true;
}

ZipFile::~ZipFile()
{
	zip_close(file_);
	isValidFile_ = false;
}

Err ZipFile::AddFile(const uint8_t* fileContent, const size_t bufferSize, const char* filename) const
{
	if (!isValidFile_)
	{
		return Err(".ZIP file is not a valid .ZIP file", error_const::ZIP_ERROR_CODE);
	}

	zip_source_t* fileBuffer = zip_source_buffer(file_, fileContent, bufferSize, 0);

	if (zip_file_add(file_, filename, fileBuffer, ZIP_FL_OVERWRITE) < 0)
	{
		zip_source_free(fileBuffer);
		return Err(std::string(zip_strerror(file_)), error_const::ZIP_ERROR_CODE);
	}

	return error_const::SUCCESS;
}

Err ZipFile::ReadFile(const char* filename, uint8_t* fileBuffer, const int bufferSize) const
{
	if (!isValidFile_)
	{
		return Err(".ZIP file is not a valid .ZIP file", error_const::ZIP_ERROR_CODE);
	}

	zip_file_t* openFile;
	if ((openFile = zip_fopen(file_, filename, 0)) == nullptr)
	{
		return Err(std::string(zip_strerror(file_)), error_const::ZIP_ERROR_CODE);
	}

	if (zip_fread(openFile, fileBuffer, bufferSize) < 0)
	{
		return Err(std::string(zip_strerror(file_)), error_const::ZIP_ERROR_CODE);
	}

	return error_const::SUCCESS;
}

Err ZipFile::DeleteFile(const char* filename) const
{
	if (!isValidFile_)
	{
		return Err(".ZIP file is not a valid .ZIP file", error_const::ZIP_ERROR_CODE);
	}

	int64_t fileIndex;

	if ((fileIndex = zip_name_locate(file_, filename, ZIP_FL_ENC_GUESS)) < 0)
	{
		return Err(std::string(zip_strerror(file_)), error_const::ZIP_ERROR_CODE);
	}

	if (zip_delete(file_, fileIndex) < 0)
	{
		return Err(std::string(zip_strerror(file_)), error_const::ZIP_ERROR_CODE);
	}

	return error_const::SUCCESS;
}

Err ZipFile::AddDir(const char* dirPath) const
{
	if (!isValidFile_)
	{
		return Err(".ZIP file is not a valid .ZIP file", error_const::ZIP_ERROR_CODE);
	}

	if (zip_dir_add(file_, dirPath, ZIP_FL_ENC_GUESS) < 0)
	{
		return Err(std::string(zip_strerror(file_)), error_const::ZIP_ERROR_CODE);
	}

	return error_const::SUCCESS;
}

bool ZipFile::IsValid() const
{
	return isValidFile_;
}


