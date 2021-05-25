#pragma once
//==============================================================================
/// 2021, Benedikt Michael
//==============================================================================
/// parser.h
/// Loads & checks the settings.
//==============================================================================

#pragma once

#include "exception.h"
#include "maths.h"
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
	struct Debris { static constexpr const char* Key{ "debris" }; };

	//window attributes
	struct ScreenWidth { static constexpr const char* Key{ "screenWidth" }; };
	struct ScreenHeight { static constexpr const char* Key{ "screenHeight" }; };
	struct PixelWidth { static constexpr const char* Key{ "pixelWidth" }; };
	struct PixelHeight { static constexpr const char* Key{ "pixelHeight" }; };
	struct FullScreen { static constexpr const char* Key{ "fullScreen" }; };

	//game attributes
	struct Velocity { static constexpr const char* Key{ "velocity" }; };
	struct AccX { static constexpr const char* Key{ "accelerationX" }; };
	struct AccY { static constexpr const char* Key{ "accelerationY" }; };
	struct MaxSpeed { static constexpr const char* Key{ "maxSpeed" }; };
	struct StarCard { static constexpr const char* Key{ "starCardinality" }; };

	//object attributes
	struct Color { static constexpr const char* Key{ "color" }; };
	struct Mass { static constexpr const char* Key{ "mass" }; };
	struct SpawnChance { static constexpr const char* Key{ "spawnChance" }; };
	struct SpawnReload { static constexpr const char* Key{ "spawnReload" }; };

	//other attributes 
	struct Name { static constexpr const char* Key{ "name" }; };


	//lookup table for translating strings to olc::Pixel
	inline const std::map<std::string, olc::Pixel> colorTable{
		{"red", olc::RED },
		{"green", olc::GREEN },
		{"blue", olc::BLUE },
		{"yellow", olc::YELLOW },
		{"cyan", olc::CYAN },
		{"magenta", olc::MAGENTA }
	};

} //end of namespace parser


class CParser
{
public:
	CParser(std::filesystem::path& currentPath);

	//////wrapper to get a ref to the rapidjason generic object for a specific nested key (implemented depth: 3)
	//e.g. syntax für depth 2:  ParserObject.parse<parser::base, parser::base::child>()
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
	template <typename PARENTKEY, typename CHILDKEY, typename GRANDCHILDKEY>
	const rapidjson::Value& parse() const
	{
		return m_settings[PARENTKEY::Key].GetObj()[CHILDKEY::Key].GetObj()[GRANDCHILDKEY::Key];
	}

	////depth 2
	//wrapped rapidjson getter
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
	template <typename PARENTKEY, typename CHILDKEY>
	std::vector<int> getVInt() const
	{
		checkKeys<PARENTKEY, CHILDKEY>();
		const auto& value{ parse<PARENTKEY, CHILDKEY>() };
		if (!value.IsArray())
			throw CException{ "Value of \"" + static_cast<std::string>(PARENTKEY::Key) + "\":\"" + static_cast<std::string>(CHILDKEY::Key) + "\" is no array.", INFO };

		std::vector<int> vec{};
		for (rapidjson::SizeType i{ 0 }; i < value.Size(); ++i)
		{
			if (!value[i].IsNumber())
				throw CException{ "The " + std::to_string(i) + "value of \"" + static_cast<std::string>(PARENTKEY::Key) + "\":\"" + static_cast<std::string>(CHILDKEY::Key) + "\" is no number.", INFO };
			else
				vec.push_back(value[i].GetInt());
		}
		return vec;
	}
	//json getter for own types
	template <typename PARENTKEY, typename CHILDKEY>
	v2d getV2D() const
	{
		checkKeys<PARENTKEY, CHILDKEY>();
		const auto& value{ parse<PARENTKEY, CHILDKEY>() };
		if (!value.IsArray())
			throw CException{ "Value of \"" + static_cast<std::string>(PARENTKEY::Key) + "\":\"" + static_cast<std::string>(CHILDKEY::Key) + "\" is no array.", INFO };
		
		v2d vec{};
		for (rapidjson::SizeType i{ 0 }; i < value.Size(); ++i)
		{
			if (i >= 2)
				//If it is too small, the remaining values will be 0
				throw CException{ "Array \"" + static_cast<std::string>(PARENTKEY::Key) + "\":\"" + static_cast<std::string>(CHILDKEY::Key) + "\" is too big for a 2D-Vector.", INFO };
			else if (!value[i].IsNumber())
				throw CException{ "The " + std::to_string(i) + "value of \"" + static_cast<std::string>(PARENTKEY::Key) + "\":\"" + static_cast<std::string>(CHILDKEY::Key) + "\" is no number.", INFO };
			else
			{
				if (i == 0)
					vec.x = value[i].GetFloat();
				else
					vec.y = value[i].GetFloat();
			}
		}
		return vec;
	}

	////depth 3
	//wrapped rapidjson getter
	template <typename PARENTKEY, typename CHILDKEY, typename GRANDCHILDKEY>
	float getFloat() const
	{
		checkKeys<PARENTKEY, CHILDKEY, GRANDCHILDKEY>();
		if (!parse<PARENTKEY, CHILDKEY, GRANDCHILDKEY>().IsNumber())
			throw CException{ "Value of \"" + static_cast<std::string>(PARENTKEY::Key) + "\":\"" + static_cast<std::string>(CHILDKEY::Key) + "\":\"" + static_cast<std::string>(GRANDCHILDKEY::Key) + "\" is no number.", INFO };

		return parse<PARENTKEY, CHILDKEY, GRANDCHILDKEY>().GetFloat();
	}
	template <typename PARENTKEY, typename CHILDKEY, typename GRANDCHILDKEY>
	int getInt() const
	{
		checkKeys<PARENTKEY, CHILDKEY, GRANDCHILDKEY>();
		if (!parse<PARENTKEY, CHILDKEY>().IsNumber())
			throw CException{ "Value of \"" + static_cast<std::string>(PARENTKEY::Key) + "\":\"" + static_cast<std::string>(CHILDKEY::Key) + "\":\"" + static_cast<std::string>(GRANDCHILDKEY::Key) + "\" is no number.", INFO };
	
		return parse<PARENTKEY, CHILDKEY, GRANDCHILDKEY>().GetInt();
	}
	template <typename PARENTKEY, typename CHILDKEY, typename GRANDCHILDKEY>
	std::string getString() const
	{
		checkKeys<PARENTKEY, CHILDKEY, GRANDCHILDKEY>();
		if (!parse<PARENTKEY, CHILDKEY>().IsString())
			throw CException{ "Value of \"" + static_cast<std::string>(PARENTKEY::Key) + "\":\"" + static_cast<std::string>(CHILDKEY::Key) + "\":\"" + static_cast<std::string>(GRANDCHILDKEY::Key) + "\" is no string.", INFO };
	
		return parse<PARENTKEY, CHILDKEY, GRANDCHILDKEY>().GetString();
	}
	template <typename PARENTKEY, typename CHILDKEY, typename GRANDCHILDKEY>
	bool getBool() const
	{
		checkKeys<PARENTKEY, CHILDKEY, GRANDCHILDKEY>();
		if (!parse<PARENTKEY, CHILDKEY, GRANDCHILDKEY>().IsBool())
			throw CException{ "Value of \"" + static_cast<std::string>(PARENTKEY::Key) + "\":\"" + static_cast<std::string>(CHILDKEY::Key) + "\":\"" + static_cast<std::string>(GRANDCHILDKEY::Key) + "\" is no boolean.", INFO };
	
		return parse<PARENTKEY, CHILDKEY, GRANDCHILDKEY>().GetBool();
	}
	template <typename PARENTKEY, typename CHILDKEY, typename GRANDCHILDKEY>
	std::vector<int> getVInt() const
	{
		checkKeys<PARENTKEY, CHILDKEY, GRANDCHILDKEY>();
		const auto& value{ parse<PARENTKEY, CHILDKEY, GRANDCHILDKEY>() };
		if (!value.IsArray())
			throw CException{ "Value of \"" + static_cast<std::string>(PARENTKEY::Key) + "\":\"" + static_cast<std::string>(CHILDKEY::Key) + "\":\"" + static_cast<std::string>(GRANDCHILDKEY::Key) + "\" is no array.", INFO };
	
		std::vector<int> vec{};
		for (rapidjson::SizeType i{ 0 }; i < value.Size(); ++i)
		{
			if (!value[i].IsNumber())
				throw CException{ "The " + std::to_string(i) + "value of \"" + static_cast<std::string>(PARENTKEY::Key) + "\":\"" + static_cast<std::string>(CHILDKEY::Key) + "\":\"" + static_cast<std::string>(GRANDCHILDKEY::Key) + "\" is no number.", INFO };
			else
				vec.push_back(value[i].GetInt());
		}
		return vec;
	}
	//json getter for own types
	template <typename PARENTKEY, typename CHILDKEY, typename GRANDCHILDKEY>
	v2d getV2D() const
	{
		checkKeys<PARENTKEY, CHILDKEY, GRANDCHILDKEY>();
		const auto& value{ parse<PARENTKEY, CHILDKEY, GRANDCHILDKEY>() };
		if (!value.IsArray())
			throw CException{ "Value of \"" + static_cast<std::string>(PARENTKEY::Key) + "\":\"" + static_cast<std::string>(CHILDKEY::Key) + "\":\"" + static_cast<std::string>(GRANDCHILDKEY::Key) + "\" is no array.", INFO };
	
		v2d vec{};
		for (rapidjson::SizeType i{ 0 }; i < value.Size(); ++i)
		{
			if (i >= 2)
				//If it is too small, the remaining values will be 0
				throw CException{ "Array \"" + static_cast<std::string>(PARENTKEY::Key) + "\":\"" + static_cast<std::string>(CHILDKEY::Key) + "\":\"" + static_cast<std::string>(GRANDCHILDKEY::Key) + "\" is too big for a 2D-Vector.", INFO };
			else if (!value[i].IsNumber())
				throw CException{ "The " + std::to_string(i) + "value of \"" + static_cast<std::string>(PARENTKEY::Key) + "\":\"" + static_cast<std::string>(CHILDKEY::Key) + "\":\"" + static_cast<std::string>(GRANDCHILDKEY::Key) + "\" is no number.", INFO };
			else
			{
				if (i == 0)
					vec.x = value[i].GetFloat();
				else
					vec.y = value[i].GetFloat();
			}
		}
		return vec;
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
	template <typename PARENTKEY, typename CHILDKEY, typename GRANDCHILDKEY>
	void checkKeys() const
	{
		if (!m_settings.HasMember(PARENTKEY::Key))
			throw CException{ "Wrong syntax or can't find key for object \"" + static_cast<std::string>(PARENTKEY::Key) + "\" in settings file.", INFO };
		if (!parse<PARENTKEY>().HasMember(CHILDKEY::Key))
			throw CException{ "Wrong syntax or can't find key for object \"" + static_cast<std::string>(PARENTKEY::Key) + "\":\"" + static_cast<std::string>(CHILDKEY::Key) + "\" in settings file.", INFO };
		if (!parse<PARENTKEY, CHILDKEY>().HasMember(GRANDCHILDKEY::Key))
			throw CException{ "Wrong syntax or can't find key for object \"" + static_cast<std::string>(PARENTKEY::Key) + "\":\"" + static_cast<std::string>(CHILDKEY::Key) + "\":\"" + static_cast<std::string>(GRANDCHILDKEY::Key) + "\" in settings file.", INFO };
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