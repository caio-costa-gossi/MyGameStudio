#pragma once
#include <cstdint>
#include <cstring>

#define LANGUAGES X(en_us) X(pt_br) X(fr_fr) X(lang_not_found)

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

	enum class IoTaskState : uint8_t
	{
		pending,
		started,
		finished,
		error
	};

	enum class AssetType : uint8_t
	{
		image,
		material,
		mesh3d,
		skeleton,
		light_map,
		animation,
		animation_state_machine,
		audio,
		audio_metadata,
		font,
		plaintext,
		script,
		level,
		tile_set,
		collider,
		particle,
		shader
	};

	inline const char* AssetTypeToString(const AssetType type)
	{
		switch (type)
		{
		case AssetType::image:
			return "image";
		case AssetType::material:
			return "material";
		case AssetType::mesh3d:
			return "mesh3d";
		case AssetType::skeleton:
			return "skeleton";
		case AssetType::light_map:
			return "light_map";
		case AssetType::animation:
			return "animation";
		case AssetType::animation_state_machine:
			return "animation_state_machine";
		case AssetType::audio:
			return "audio";
		case AssetType::audio_metadata:
			return "audio_metadata";
		case AssetType::font:
			return "font";
		case AssetType::plaintext:
			return "plaintext";
		case AssetType::script:
			return "script";
		case AssetType::level:
			return "level";
		case AssetType::tile_set:
			return "tile_set";
		case AssetType::collider:
			return "collider";
		case AssetType::particle:
			return "particle";
		case AssetType::shader:
			return "shader";
		default:
			return "undefined";
		}
	}
}