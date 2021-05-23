#pragma once
//==============================================================================
/// 2021, Benedikt Michael
//==============================================================================
/// parser.h
/// Loads & checks the settings.
//==============================================================================

#pragma once

#include "rapidjson/document.h"

#include <filesystem>
#include <vector>


//helper classes for rapidjson wrapper, and for having one place to define all setting keys
namespace parser
{
	struct Window { static constexpr const char* Key{ "window" };
		struct Name { static constexpr const char* Key{ "name" }; };
		struct ScreenWidth { static constexpr const char* Key{ "screenWidth" }; };
		struct ScreenHeight { static constexpr const char* Key{ "screenHeight" }; };
		struct PixelWidth { static constexpr const char* Key{ "pixelWidth" }; };
		struct PixelHeight { static constexpr const char* Key{ "pixelHeight" }; };
		struct FullScreen { static constexpr const char* Key{ "fullScreen" }; };
		struct StartMass { static constexpr const char* Key{ "startMass" }; };
	};
	struct Game { static constexpr const char* Key{ "game" };
		struct StartMass { static constexpr const char* Key{ "startMass" }; };
	};

} //end of namespace parser


class CParser
{
public:
	CParser(std::filesystem::path& currentPath);

	//wrapper to get a ref to the rapidjason generic object for a specific nested key (with implemented depth 3)
	//syntax für depth 2:  ParserObject.parse<parser::base, parser::base::child>()
	template <typename KEY>
	const auto& parse() const
	{
		return m_settings[KEY::Key];
	}
	template <typename PARENTKEY, typename CHILDKEY>
	const auto& parse() const
	{
		return m_settings[PARENTKEY::Key].GetObj()[CHILDKEY::Key];
	}
	template <typename PARENTKEY, typename CHILDKEY, typename GRANDCHILDKEY>
	const auto& parse() const
	{
		return m_settings[PARENTKEY::Key].GetObj()[CHILDKEY::Key].GetObj()[GRANDCHILDKEY::Key];
	}

private:
	//settings path
	std::filesystem::path m_SettingsPath;

	//the rapidjson object where all settings are beeing parsed to
	rapidjson::Document m_settings;

	//start of parsing
	void start();
	//read all setting files and write settings into a rapidjson::Document
	void readFiles();
};