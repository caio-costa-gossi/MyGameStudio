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

    static bool HasValue(std::vector<std::unique_ptr<char[]>>& vector, const char* value);

public:
	static Err Startup(const char* translationFilePath, const char* stringIdColumnName);
	static const char* GetLocalizedStringInLanguage(const char* stringId, const char* languageName);
    static const char* GetLocalizedString(const char* stringId);

	static enums::Language GetLanguageSet();
    static Err SetLanguage(enums::Language language);
};

namespace string_const
{
    const auto G_STARTUP_MSG = "startup_msg";
    const auto G_SYS_INIT = "sys_init";
    const auto G_SYS_INIT_READY = "sys_init_ready";
    const auto G_SYS_SHUTDOWN = "sys_shutdown";
    const auto G_TOTAL_RUN_TIME = "total_run_time";
    const auto G_LANG_SELECTED = "lang_selected";
    const auto G_HELP_COMMAND = "help_command";
    const auto G_QUIT_CONSOLE = "quit_console";
    const auto G_LANGUAGE_CHANGED = "language_changed";
    const auto G_LANGUAGE_NOT_FOUND = "language_not_found";
    const auto G_INVALID_PARAMETER = "invalid_parameter";
    const auto G_USE_CHLANG = "use_chlang";
    const auto G_ASSET_IMPORT = "asset_import";
    const auto G_NOT_TRIANGULATED = "not_triangulated";
}