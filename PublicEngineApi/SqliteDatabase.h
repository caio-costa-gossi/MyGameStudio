#pragma once
#include <vector>

#include "Err.h"
#include "sqlite3.h"

class SqliteDatabase
{
private:
	bool isDbOpen_ = false;
	sqlite3* database_ = nullptr;

public:
	SqliteDatabase() = default;
	~SqliteDatabase() { if (isDbOpen_) CloseDb(); }

	Err OpenDb(const char* filename);
	Err CloseDb();
	Err ExecuteNonQuery(const char* sqlStatement) const;
	Err ExecuteInsert(const char* insertStatement, int64_t& rowId) const;

	bool IsDbOpen() const;

	template<typename T>
	T ExecuteQuerySingle(const char* sqlStatement) const
	{
		static_assert(std::is_invocable_r<T, decltype(&T::FromStmt), sqlite3_stmt*>().value,
			"Error: class T must have a T::FromStmt(sqlite3_stmt*) method.");

		if (!isDbOpen_)
		{
			std::cout << "Error! Database is closed!";
			return T();
		}

		sqlite3_stmt* stmt = nullptr;

		if (sqlite3_prepare_v2(database_, sqlStatement, -1, &stmt, nullptr))
		{
			std::cout << "Error preparing statement for DB query!";
			return T();
		}

		sqlite3_step(stmt);

		T returnValue = T::FromStmt(stmt);
		sqlite3_finalize(stmt);

		return returnValue;
	}

	template<typename T>
	std::vector<T> ExecuteQuery(const char* sqlStatement) const
	{
		static_assert(std::is_invocable_r<T, decltype(&T::FromStmt), sqlite3_stmt*>().value,
			"Error: class T must have a T::FromStmt(sqlite3_stmt*) method.");

		std::vector<T> results;

		if (!isDbOpen_)
		{
			std::cout << "Error! Database is closed!";
			return results;
		}
		
		results.reserve(256);

		sqlite3_stmt* stmt = nullptr;

		if (sqlite3_prepare_v2(database_, sqlStatement, -1, &stmt, nullptr))
		{
			std::cout << "Error preparing statement for DB query!";
			return results;
		}

		while (sqlite3_step(stmt) == SQLITE_ROW)
		{
			results.emplace_back(T::FromStmt(stmt));
		}

		sqlite3_finalize(stmt);
		return results;
	}
};
