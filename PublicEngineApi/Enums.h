#pragma once
#include <cstdint>
#include <cstring>

#define LANGUAGES X(en_us) X(pt_br) X(fr_fr) X(lang_not_found)

#define ASSET_TYPES \
X(image) \
X(material) \
X(mesh3d) \
X(skeleton) \
X(light_map) \
X(animation) \
X(animation_state_machine) \
X(audio) \
X(audio_metadata) \
X(video) \
X(font) \
X(plaintext) \
X(script) \
X(header) \
X(level) \
X(tile_set) \
X(collider) \
X(particle) \
X(shader) \
X(undefined)

namespace enums
{
	// Enums

	enum class Language : uint8_t
	{
		#define X(name) name,
		LANGUAGES
		#undef X
	};

	enum class IoTaskState : uint8_t
	{
		pending,
		started,
		finished,
		error
	};

	enum class AssetType : uint8_t
	{
		#define X(asset_type) asset_type,
		ASSET_TYPES
		#undef X
	};

	enum class AssetSaveType : uint8_t
	{
		save_to_zip,
		save_to_assets,
		save_to_game
	};

	enum class ConsoleMessageType : uint8_t
	{
		info,
		warning,
		error,
		critical_error
	};

	// Functions

	inline const char* LanguageToString(Language language)
	{
		switch (language)
		{
			#define X(name) case Language::name: return #name; break;
			LANGUAGES
			#undef X
		default:
			return "N/A";
		}
	}

	inline Language StringToLanguage(const char* string)
	{
		#define X(name) if (std::strcmp(string,#name) == 0) return Language::name;
		LANGUAGES
		#undef X
		return Language::lang_not_found;
	}

	inline const char* AssetTypeToString(const AssetType type)
	{
		switch (type)
		{
		#define X(asset_type) case AssetType::asset_type: return #asset_type;
		ASSET_TYPES
		#undef X
		default:
			return "undefined";
		}
	}

	inline AssetType StringToAssetType(const char* string)
	{
		#define X(asset_type) if (std::strcmp(string,#asset_type) == 0) return AssetType::asset_type;
		ASSET_TYPES
		#undef X
		return AssetType::undefined;
	}
}