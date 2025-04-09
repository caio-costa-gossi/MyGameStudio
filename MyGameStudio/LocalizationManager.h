#pragma once
#include <unordered_map>

#include "CsvParser.h"
#include "Enums.h"
#include "Err.h"

struct CStringPairHash {
    std::size_t operator()(const std::pair<const char*, const char*>& p) const {
        return std::hash<std::string>()(std::string(p.first)) ^
            std::hash<std::string>()(std::string(p.second));
    }
};

struct CStringPairEqual {
    bool operator()(const std::pair<const char*, const char*>& a,
        const std::pair<const char*, const char*>& b) const {
        return std::strcmp(a.first, b.first) == 0 &&
            std::strcmp(a.second, b.second) == 0;
    }
};

class LocalizationManager
{
private:
	static std::unordered_map<std::pair<const char*, const char*>, std::unique_ptr<char[]>, CStringPairHash, CStringPairEqual> translationTable_;
	static std::vector<std::unique_ptr<char[]>> languageList_;
    static std::vector<std::unique_ptr<char[]>> stringIdList_;

	static const char* stringIdColumnName_;
    static enums::Language languageSet_;

	static Err ValidateSpreadsheetFormat(CsvParser& parser);
	static Err PopulateTable(CsvParser& parser);

public:
	static Err Startup(const char* translationFilePath, const char* stringIdColumnName);
	static const char* GetLocalizedStringInLanguage(const char* stringId, const char* languageName);
    static const char* GetLocalizedString(const char* stringId);

	static enums::Language GetLanguageSet();
    static Err SetLanguage(enums::Language language);
};
