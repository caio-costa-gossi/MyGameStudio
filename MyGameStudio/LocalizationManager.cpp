#include "LocalizationManager.h"

#include "CsvParser.h"

Err LocalizationManager::Startup(const char* translationFilePath, const char* stringIdColumnName)
{
	stringIdColumnName_ = stringIdColumnName;

	CsvParser parser(translationFilePath);

	Err err = ValidateSpreadsheetFormat(parser);
	if (err.Code() != 0)
		return err;

	err = PopulateTable(parser);
	if (err.Code() != 0)
		return err;

	return error_const::SUCCESS;
}

const char* LocalizationManager::GetLocalizedString(const char* stringId, const char* languageName)
{
	return translationTable_[{stringId, languageName}].get();
}

Err LocalizationManager::ValidateSpreadsheetFormat(CsvParser& parser)
{
	std::vector<std::unique_ptr<char[]>>& headers = parser.GetHeaders();

	bool stringIdPresent = false;

	auto it = std::vector<std::unique_ptr<char[]>>::iterator();
	for (it = headers.begin(); it != headers.end(); ++it)
	{
		if (std::strcmp((*it).get(), stringIdColumnName_) == 0)
			stringIdPresent = true;
		else
			languageList_.push_back((*it).get());
	}

	if (!stringIdPresent)
		return error_const::INVALID_FORMAT;

	return error_const::SUCCESS;
}

Err LocalizationManager::PopulateTable(CsvParser& parser)
{
	std::vector<std::unique_ptr<char[]>>& stringIdList = parser.GetColumn(stringIdColumnName_);

	for (uint16_t id = 0; id < stringIdList.size(); ++id)
	{
		auto languageIt = std::vector<const char*>::iterator();
		for (languageIt = languageList_.begin(); languageIt != languageList_.end(); ++languageIt)
		{
			const char* stringId = stringIdList[id].get();
			const char* languageName = *languageIt;
			const char* stringValue = parser.GetValue(languageName, id);

			auto pStringId = std::make_unique<char[]>(strlen(stringId) + 1);
			auto pLanguageName = std::make_unique<char[]>(strlen(languageName) + 1);
			auto pStringValue = std::make_unique<char[]>(strlen(stringValue) + 1);

			strcpy_s(pStringId.get(), strlen(stringId) + 1, stringId);
			strcpy_s(pLanguageName.get(), strlen(languageName) + 1, languageName);
			strcpy_s(pStringValue.get(), strlen(stringValue) + 1, stringValue);

			translationTable_.insert({ std::make_pair(pStringId.get(), pLanguageName.get()), std::move(pStringValue) });

			stringStorage_.push_back(std::move(pStringId));
			stringStorage_.push_back(std::move(pLanguageName));
		}
	}

	return error_const::SUCCESS;
}

auto LocalizationManager::translationTable_ = std::unordered_map<std::pair<const char*, const char*>, std::unique_ptr<char[]>, CStringPairHash, CStringPairEqual>();
auto LocalizationManager::languageList_ = std::vector<const char*>();
auto LocalizationManager::stringStorage_ = std::vector<std::unique_ptr<char[]>>();
const char* LocalizationManager::stringIdColumnName_ = "";