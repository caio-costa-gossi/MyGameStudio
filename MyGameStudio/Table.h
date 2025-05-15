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
	uint32_t curRow_ = 0;

public:
	Table(const std::vector<std::unique_ptr<char[]>>& colNames, ValueType* itemArray, const uint32_t colCount, const uint32_t rowCount) : itemArray_(itemArray), colCount_(colCount), rowCount_(rowCount)
	{
		colNames_.reserve(colCount);

		for (const std::unique_ptr<char[]>& name : colNames)
			colNames_.emplace_back(std::string(name.get()));
	}

	bool IsTableEnd() const { return curRow_ >= rowCount_; }

	uint32_t ColCount() const { return colCount_; }

	ValueType* GetNextRow()
	{
		return &itemArray_[curRow_++ * colCount_];
	}

	const std::vector<std::string>& GetColNames() const { return colNames_; }
};
