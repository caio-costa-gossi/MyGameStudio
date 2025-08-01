#pragma once
#include <cstdint>
#include <cstring>

#define LANGUAGES X(en_us) X(pt_br) X(fr_fr) X(lang_not_found)

#define ASSET_TYPES \
X(image) \
X(material) \
X(mesh3d_glb) \
X(mesh3d_gltf) \
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

	enum class GameExeArguments : uint8_t
	{
		exe_path = 0,
		use_sdl,
		deadzone,
		verbosity_level,
		disable_log_channels,
		debug_info_type,
		debug_pipe_name
	};

	enum class ConsoleMessageType : uint8_t
	{
		info,
		warning,
		error,
		critical_error
	};

	enum class ConsoleMessageSender : uint8_t
	{
		undefined = 0,
		game = 1,
		console = 2,
		loop = 3,
		object = 4,
		window = 5,
		debugger = 6,
		event = 7,
		localization = 8,
		io = 9,
		input = 10,
		render = 11,
		physics = 12,
		animation = 13,
		aux = 14,
		asset = 15,
		camera = 16
	};

	enum GameDebugType : uint8_t
	{
		no_debug_from_child = 0,
		input_debug = 1
	};

	enum TextureWrapBehavior : uint8_t
	{
		repeat = 0,
		mirror_repeat = 1,
		clamp_edge = 2,
		clamp_border = 3
	};

	enum TextureMap : uint8_t
	{
		base_color = 0,
		normal = 1,
		metallic_roughness = 2,
		occlusion = 3,
		emissive = 4
	};

	enum MatrixDim : uint8_t
	{
		m1x1 = 1,
		m2x2 = 2,
		m3x3 = 3,
		m4x4 = 4,
		m2x1 = 5,
		m1x2 = 6,
		m2x3 = 7,
		m3x2 = 8,
		m3x4 = 9,
		m4x3 = 10
	};

	enum CameraType : uint8_t
	{
		orthogonal = 0,
		perspective = 1
	};

	enum CameraControlScheme : uint8_t
	{
		free_cam = 0,
		bound = 1
	};

	enum LightType : uint8_t
	{
		directional = 0,
		point = 1,
		spot = 2
	};

	enum ImageFormat : uint8_t
	{
		png = 0,
		jpeg = 1
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

	inline const char* MessageSenderToString(const enums::ConsoleMessageSender sender)
	{
		switch (sender)
		{
		case enums::ConsoleMessageSender::undefined:
			return "UNDEFINED";
		case enums::ConsoleMessageSender::game:
			return "GAME";
		case enums::ConsoleMessageSender::console:
			return "CONSOLE";
		case enums::ConsoleMessageSender::loop:
			return "LOOP";
		case enums::ConsoleMessageSender::object:
			return "OBJECTS";
		case enums::ConsoleMessageSender::window:
			return "WINDOW";
		case enums::ConsoleMessageSender::debugger:
			return "DEBUGGER";
		case enums::ConsoleMessageSender::event:
			return "EVENTS";
		case enums::ConsoleMessageSender::localization:
			return "LOCALIZATION";
		case enums::ConsoleMessageSender::io:
			return "IO";
		case enums::ConsoleMessageSender::input:
			return "INPUT";
		case enums::ConsoleMessageSender::render:
			return "RENDER";
		case enums::ConsoleMessageSender::physics:
			return "PHYSICS";
		case enums::ConsoleMessageSender::animation:
			return "ANIMATION";
		case enums::ConsoleMessageSender::aux:
			return "AUXILIARY";
		case enums::ConsoleMessageSender::asset:
			return "ASSET";
		case enums::ConsoleMessageSender::camera:
			return "CAMERA";
		default:
			return "NAME NOT FOUND";
		}
	}
}