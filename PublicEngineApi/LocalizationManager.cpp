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

const char* LocalizationManager::GetLocalizedStringInLanguage(const char* stringId, const char* languageName)
{
	const char* string = translationTable_[{stringId, languageName}].get();

	if (string != nullptr)
		return string;

	return "N/A";
}

const char* LocalizationManager::GetLocalizedString(const char* stringId)
{
	const char* string = translationTable_[{stringId, enums::LanguageToString(languageSet_)}].get();

	if (string != nullptr)
		return string;

	return "N/A";
}

enums::Language LocalizationManager::GetLanguageSet()
{
	return languageSet_;
}

Err LocalizationManager::SetLanguage(const enums::Language language)
{
	languageSet_ = language;
	return error_const::SUCCESS;
}

bool LocalizationManager::HasValue(std::vector<std::unique_ptr<char[]>>& vector, const char* value)
{
	auto it = std::vector<std::unique_ptr<char[]>>::iterator();

	for (it = vector.begin(); it != vector.end(); ++it)
	{
		if (std::strcmp((*it).get(), value) == 0)
			return true;
	}

	return false;
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
			languageList_.push_back(std::move(*it));
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
		std::unique_ptr<char[]>& pStringId = stringIdList[id];

		auto languageIt = std::vector<std::unique_ptr<char[]>>::iterator();
		for (languageIt = languageList_.begin(); languageIt != languageList_.end(); ++languageIt)
		{
			std::unique_ptr<char[]>& pLanguageName = *languageIt;
			std::unique_ptr<char[]>& pStringValue = parser.GetValue(pLanguageName.get(), id);

			translationTable_.insert({ std::make_pair(pStringId.get(), pLanguageName.get()), std::move(pStringValue) });
		}

		stringIdList_.push_back(std::move(stringIdList[id]));
	}

	return error_const::SUCCESS;
}

auto LocalizationManager::translationTable_ = std::unordered_map<std::pair<const char*, const char*>, std::unique_ptr<char[]>, CStringPairHash, CStringPairEqual>();
auto LocalizationManager::languageList_ = std::vector<std::unique_ptr<char[]>>();
auto LocalizationManager::stringIdList_ = std::vector<std::unique_ptr<char[]>>();

const char* LocalizationManager::stringIdColumnName_ = "";
enums::Language LocalizationManager::languageSet_ = enums::Language::en_us;