#include "SystemFileHelper.h"

Err SystemFileHelper::RemoveFile(const char* filepath)
{
	if (std::remove(filepath))
		return error_const::FILE_DELETION_FAIL;

	return error_const::SUCCESS;
}
