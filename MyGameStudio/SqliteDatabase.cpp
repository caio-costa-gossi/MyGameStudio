#include "SqliteDatabase.h"

Err SqliteDatabase::OpenDb(const char* filename)
{
	if (isDbOpen_)
		return error_const::REDUNDANT_CALL;

	if (sqlite3_open(filename, &database_))
	{
		std::cout << "Error opening database: " << sqlite3_errmsg(database_) << "\n";
		return error_const::GENERIC_EXCEPTION;
	}

	isDbOpen_ = true;
	return error_const::SUCCESS;
}

Err SqliteDatabase::CloseDb()
{
	if (!isDbOpen_)
		return error_const::REDUNDANT_CALL;

	sqlite3_close(database_);
	isDbOpen_ = false;
	return error_const::SUCCESS;
}

Err SqliteDatabase::ExecuteNonQuery(const char* sqlStatement) const
{
	if (!isDbOpen_)
		return error_const::DB_CLOSED;

	char* errorMsg;
	if (sqlite3_exec(database_, sqlStatement, nullptr, nullptr, &errorMsg))
		return Err(errorMsg, 10);

	return error_const::SUCCESS;
}

bool SqliteDatabase::IsDbOpen() const
{
	return isDbOpen_;
}
