#pragma once
#include <cstdint>
#include <cstring>

#define LANGUAGES X(en_us) X(pt_br) X(fr_fr)

namespace enums
{
	enum class Language : uint8_t
	{
		#define X(name) name,
		LANGUAGES
		#undef X
	};

	inline const char* LanguageToString(Language language)
	{
		switch (language)
		{
		#define X(name) case Language::name: return #name; break;
		LANGUAGES
		#undef X
		default:
			return "en_us";
		}
	}

	inline Language StringToLanguage(const char* string)
	{
		#define X(name) if (std::strcmp(string,#name) == 0) return Language::name;
		LANGUAGES
		#undef X
	}
}