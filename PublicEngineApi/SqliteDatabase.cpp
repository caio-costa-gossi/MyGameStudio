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

Err SqliteDatabase::ExecuteQuerySingleValue(const char* sqlStatement, int32_t& value) const
{
	if (!isDbOpen_)
		return error_const::DB_CLOSED;

	sqlite3_stmt* stmt = nullptr;

	if (sqlite3_prepare_v2(database_, sqlStatement, -1, &stmt, nullptr))
		return Err("Error preparing statement for DB query!", error_const::DB_ERROR_CODE);

	if (sqlite3_step(stmt) == SQLITE_DONE)
		return Err("No entries found in the table", error_const::DB_ERROR_CODE);

	value = sqlite3_column_int(stmt, 0);
	sqlite3_finalize(stmt);

	return error_const::SUCCESS;
}

Err SqliteDatabase::ExecuteNonQuery(const char* sqlStatement) const
{
	if (!isDbOpen_)
		return error_const::DB_CLOSED;

	char* errorMsg;
	if (sqlite3_exec(database_, sqlStatement, nullptr, nullptr, &errorMsg))
		return Err(errorMsg, error_const::DB_ERROR_CODE);

	return error_const::SUCCESS;
}

Err SqliteDatabase::ExecuteInsert(const char* insertStatement, int64_t& rowId) const
{
	if (!isDbOpen_)
	{
		rowId = -1;
		return error_const::DB_CLOSED;
	}

	char* errorMsg;
	if (sqlite3_exec(database_, insertStatement, nullptr, nullptr, &errorMsg))
	{
		rowId = -1;
		return Err(errorMsg, error_const::DB_ERROR_CODE);
	}

	rowId = sqlite3_last_insert_rowid(database_);
	return error_const::SUCCESS;
}

bool SqliteDatabase::IsDbOpen() const
{
	return isDbOpen_;
}
