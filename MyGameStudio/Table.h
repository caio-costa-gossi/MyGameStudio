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
	uint32_t Current() const { return curRowCol_; }
	bool IsRowMajor() const { return isRowMajor_; }

	void ResetCursor() { curRowCol_ = 0; }

	ValueType* GetNext()
	{
		if (isRowMajor_)
			return &itemArray_[curRowCol_++ * colCount_];

		return &itemArray_[curRowCol_++ * rowCount_];
	}

	void Transpose()
	{
		ValueType* tempArray = new ValueType[rowCount_ * colCount_];

		for (uint32_t row = 0; row < rowCount_; ++row)
		{
			for (uint32_t col = 0; col < colCount_; ++col)
			{
				if (isRowMajor_)
					tempArray[col * rowCount_ + row] = itemArray_[row * colCount_ + col];
				else
					tempArray[row * colCount_ + col] = itemArray_[col * rowCount_ + row];
			}
		}

		std::copy(tempArray, tempArray + rowCount_ * colCount_, itemArray_);
		delete[] tempArray;

		curRowCol_ = 0;
		isRowMajor_ = !isRowMajor_;
	}
};
