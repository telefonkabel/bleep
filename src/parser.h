#pragma once
//==============================================================================
/// 2021, Benedikt Michael
//==============================================================================
/// parser.h
/// Loads & checks the settings.
//==============================================================================

#pragma once

#include "exception.h"
#include "rapidjson/document.h"
#include "olcPixelGameEngine.h"

#include <filesystem>
#include <vector>


//helper objects for rapidjson wrapper, and for having one place to define all setting keys
namespace parser
{
	//objects
	struct Window { static constexpr const char* Key{ "window" }; };
	struct Game { static constexpr const char* Key{ "game" }; };
	struct Player { static constexpr const char* Key{ "player" }; };

	//window attributes
	struct ScreenWidth { static constexpr const char* Key{ "screenWidth" }; };
	struct ScreenHeight { static constexpr const char* Key{ "screenHeight" }; };
	struct PixelWidth { static constexpr const char* Key{ "pixelWidth" }; };
	struct PixelHeight { static constexpr const char* Key{ "pixelHeight" }; };
	struct FullScreen { static constexpr const char* Key{ "fullScreen" }; };

	//game attributes
	struct Velocity { static constexpr const char* Key{ "velocity" }; };

	//object attributes
	struct Color { static constexpr const char* Key{ "color" }; };
	struct Mass { static constexpr const char* Key{ "mass" }; };

	//other attributes 
	struct Name { static constexpr const char* Key{ "name" }; };


	//lookup table for translating strings to olc::Pixel
	inline const std::map<std::string, olc::Pixel> colorTable{
		{"red", olc::RED },
		{"green", olc::GREEN },
		{"blue", olc::BLUE },
		{"yellow", olc::YELLOW },
		{"cyan", olc::CYAN },
		{"magenta", olc::MAGENTA },
	};

} //end of namespace parser


class CParser
{
public:
	CParser(std::filesystem::path& currentPath);

	//wrapper to get a ref to the rapidjason generic object for a specific nested key (currently only needed depth: 2)
	//syntax für depth 2:  ParserObject.parse<parser::base, parser::base::child>()
	template <typename KEY>
	const rapidjson::Value& parse() const
	{
		return m_settings[KEY::Key];
	}
	template <typename PARENTKEY, typename CHILDKEY>
	const rapidjson::Value& parse() const
	{
		return m_settings[PARENTKEY::Key].GetObj()[CHILDKEY::Key];
	}
	//overwritten/own rapidjson getter
	template <typename PARENTKEY, typename CHILDKEY>
	float getFloat() const
	{
		checkKeys<PARENTKEY, CHILDKEY>();
		if(!parse<PARENTKEY, CHILDKEY>().IsNumber())
			throw CException {"Value of \"" + static_cast<std::string>(PARENTKEY::Key) + "\":\"" + static_cast<std::string>(CHILDKEY::Key) + "\" is no number.", INFO };

		return parse<PARENTKEY, CHILDKEY>().GetFloat();
	}
	template <typename PARENTKEY, typename CHILDKEY>
	int getInt() const
	{
		checkKeys<PARENTKEY, CHILDKEY>();
		if (!parse<PARENTKEY, CHILDKEY>().IsNumber())
			throw CException{ "Value of \"" + static_cast<std::string>(PARENTKEY::Key) + "\":\"" + static_cast<std::string>(CHILDKEY::Key) + "\" is no number.", INFO };

		return parse<PARENTKEY, CHILDKEY>().GetInt();
	}
	template <typename PARENTKEY, typename CHILDKEY>
	std::string getString() const
	{
		checkKeys<PARENTKEY, CHILDKEY>();
		if (!parse<PARENTKEY, CHILDKEY>().IsString())
			throw CException{ "Value of \"" + static_cast<std::string>(PARENTKEY::Key) + "\":\"" + static_cast<std::string>(CHILDKEY::Key) + "\" is no string.", INFO };

		return parse<PARENTKEY, CHILDKEY>().GetString();
	}
	template <typename PARENTKEY, typename CHILDKEY>
	bool getBool() const
	{
		checkKeys<PARENTKEY, CHILDKEY>();
		if (!parse<PARENTKEY, CHILDKEY>().IsBool())
			throw CException{ "Value of \"" + static_cast<std::string>(PARENTKEY::Key) + "\":\"" + static_cast<std::string>(CHILDKEY::Key) + "\" is no boolean.", INFO };

		return parse<PARENTKEY, CHILDKEY>().GetBool();
	}
	//helper for validity checks
	template <typename PARENTKEY, typename CHILDKEY>
	void checkKeys() const
	{
		if (!m_settings.HasMember(PARENTKEY::Key))
			throw CException{ "Wrong syntax or can't find key for object \"" + static_cast<std::string>(PARENTKEY::Key) + "\" in settings file.", INFO };
		if (!parse<PARENTKEY>().HasMember(CHILDKEY::Key))
			throw CException{ "Wrong syntax or can't find key for object \"" + static_cast<std::string>(PARENTKEY::Key) + "\":\"" + static_cast<std::string>(CHILDKEY::Key) + "\" in settings file.", INFO };
	}

	//helper fct to determine player color
	olc::Pixel color(std::string color) const;

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