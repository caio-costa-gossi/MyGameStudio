#pragma once
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

template<typename ValueType>
class Table
{
private:
	std::vector<std::string> colNames_;
	ValueType* itemArray_;

	uint32_t colCount_;
	uint32_t rowCount_;
	uint32_t curRowCol_ = 0;

	bool isRowMajor_;

public:
	Table(const std::vector<std::unique_ptr<char[]>>& colNames, ValueType* itemArray, const uint32_t colCount, const uint32_t rowCount, bool isRowMajor = true) : itemArray_(itemArray), colCount_(colCount), rowCount_(rowCount), isRowMajor_(isRowMajor)
	{
		colNames_.reserve(colCount);

		for (const std::unique_ptr<char[]>& name : colNames)
			colNames_.emplace_back(std::string(name.get()));
	}

	bool IsTableEnd() const
	{
		if (isRowMajor_)
			return curRowCol_ >= rowCount_;

		return curRowCol_ >= colCount_;
	}

	const std::vector<std::string>& GetColNames() const { return colNames_; }

	uint32_t ColCount() const { return colCount_; }
	uint32_t RowCount() const { return rowCount_; }

	ValueType* GetNext()
	{
		if (isRowMajor_)
			return &itemArray_[curRowCol_++ * colCount_];

		return &itemArray_[curRowCol_++ * rowCount_];
	}
};
