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
    const auto G_USE_IMPORT = "use_import";
    const auto G_ASSET_REGISTER_DB = "asset_register_db";
    const auto G_ASSET_REGISTER_DB_SUCCESS = "asset_register_db_success";
    const auto G_ASSET_CLEAR_DB = "asset_clear_db";
    const auto G_ASSET_CLEAR_DB_CONFIRMATION = "asset_clear_db_confirmation";
    const auto G_USER_SETUP = "user_setup";
    const auto G_PREPARE_BUILD_GAME = "prepare_build_game";
    const auto G_BUILD_GAME = "build_game";
    const auto G_RUN_GAME = "run_game";
    const auto G_QUIT_GAME = "quit_game";
    const auto G_BUILD_SUCCESSFUL = "build_successful";
    const auto G_GAME_BIN_NOT_FOUND = "game_bin_not_found";
    const auto G_CONFIRM_CLRDB = "confirm_clrdb";
    const auto G_YN_QUESTION_DEFAULT_N = "yn_question_default_n";
    const auto G_YN_QUESTION_DEFAULT_Y = "yn_question_default_y";
    const auto G_USE_DASSET = "use_dasset";
    const auto G_CONFIRM_DASSET = "confirm_dasset";
    const auto G_DASSET_SUCCESS = "dasset_success";
    const auto G_STRING_NOT_NUMBER = "string_not_number";
    const auto G_CANCELED_BY_USER = "canceled_by_user";
    const auto G_ASSETDB_CLEARED = "assetdb_cleared";
    const auto G_ASSET_DELETION_SUCCESS = "asset_deletion_success";
    const auto G_USE_RUN = "use_run";
}