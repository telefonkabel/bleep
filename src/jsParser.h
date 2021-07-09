#pragma once
//==============================================================================
/// \copyright (c) 2021, Benedikt Michael
//==============================================================================
/// \file jsParser.h
/// \brief Loads & checks the settings.
//==============================================================================

#pragma once

#include "thirdParty/rapidjson/document.h"

#include "helper/maths.h"
#include "helper/exception.h"
#include "thirdParty/olcPixelGameEngine.h"

#include <filesystem>
#include <vector>


namespace bleep
{

	//helper objects for RapidJSON wrapper, and for having one place to define all setting keys
	namespace parser
	{
		//===== objects =====
		/// \brief Minor struct holding a string key for template usage.
		struct Window { static constexpr const char* Key{ "window" }; };
		/// \brief Minor struct holding a string key for template usage.
		struct Game { static constexpr const char* Key{ "game" }; };
		/// \brief Minor struct holding a string key for template usage.
		struct Player { static constexpr const char* Key{ "player" }; };
		/// \brief Minor struct holding a string key for template usage.
		struct Debris { static constexpr const char* Key{ "debris" }; };
		/// \brief Minor struct holding a string key for template usage.
		struct Asteroid { static constexpr const char* Key{ "asteroid" }; };
		/// \brief Minor struct holding a string key for template usage.
		struct Missile { static constexpr const char* Key{ "missile" }; };
		/// \brief Minor struct holding a string key for template usage.
		struct Hawking { static constexpr const char* Key{ "hawking" }; };

		//===== window attributes =====
		/// \brief Minor struct holding a string key for template usage.
		struct ScreenWidth { static constexpr const char* Key{ "screenWidth" }; };
		/// \brief Minor struct holding a string key for template usage.
		struct ScreenHeight { static constexpr const char* Key{ "screenHeight" }; };
		/// \brief Minor struct holding a string key for template usage.
		struct PixelWidth { static constexpr const char* Key{ "pixelWidth" }; };
		/// \brief Minor struct holding a string key for template usage.
		struct PixelHeight { static constexpr const char* Key{ "pixelHeight" }; };
		/// \brief Minor struct holding a string key for template usage.
		struct FullScreen { static constexpr const char* Key{ "fullScreen" }; };

		//=====game attributes =====
		/// \brief Minor struct holding a string key for template usage.
		struct Velocity { static constexpr const char* Key{ "velocity" }; };
		/// \brief Minor struct holding a string key for template usage.
		struct Acceleration { static constexpr const char* Key{ "acceleration" }; };
		/// \brief Minor struct holding a string key for template usage.
		struct MaxSpeed { static constexpr const char* Key{ "maxSpeed" }; };
		/// \brief Minor struct holding a string key for template usage.
		struct StarCard { static constexpr const char* Key{ "starCardinality" }; };

		//===== object attributes =====
		/// \brief Minor struct holding a string key for template usage.
		struct Color { static constexpr const char* Key{ "color" }; };
		/// \brief Minor struct holding a string key for template usage.
		struct Mass { static constexpr const char* Key{ "mass" }; };
		/// \brief Minor struct holding a string key for template usage.
		struct Speed { static constexpr const char* Key{ "speed" }; };
		/// \brief Minor struct holding a string key for template usage.
		struct SpawnChance { static constexpr const char* Key{ "spawnChance" }; };
		/// \brief Minor struct holding a string key for template usage.
		struct SpawnReload { static constexpr const char* Key{ "spawnReload" }; };
		/// \brief Minor struct holding a string key for template usage.
		struct SpawnTime { static constexpr const char* Key{ "spawnTime" }; };
		/// \brief Minor struct holding a string key for template usage.
		struct Sprite { static constexpr const char* Key{ "sprite" }; };
		/// \brief Minor struct holding a string key for template usage.
		struct Radius { static constexpr const char* Key{ "radius" }; };

		//===== sound attributes =====
		/// \brief Minor struct holding a string key for template usage.
		struct Music { static constexpr const char* Key{ "music" }; };
		/// \brief Minor struct holding a string key for template usage.
		struct Jet { static constexpr const char* Key{ "jet" }; };
		/// \brief Minor struct holding a string key for template usage.
		struct Crash { static constexpr const char* Key{ "crash" }; };
		/// \brief Minor struct holding a string key for template usage.
		struct Shot { static constexpr const char* Key{ "shot" }; };
		/// \brief Minor struct holding a string key for template usage.
		struct Impact { static constexpr const char* Key{ "impact" }; };

		//===== other attributes =====
		/// \brief Minor struct holding a string key for template usage.
		struct Name { static constexpr const char* Key{ "name" }; };


		/// \brief Lookup table for translating color strings to olc::Pixel.
		inline const std::map<std::string, olc::Pixel> colorTable{
			{"red", olc::RED },
			{"green", olc::GREEN },
			{"blue", olc::BLUE },
			{"yellow", olc::YELLOW },
			{"cyan", olc::CYAN },
			{"magenta", olc::MAGENTA }
		};

	} //end of namespace parser


	/// \brief Parser class.
	/// This handles the settings read out of JSON files.
	class CParser
	{
	public:
		/// \brief Constructor of parser class.
		/// @param currentPath Execution path of the application.
		CParser(std::filesystem::path& currentPath);

		//===== depth 2 =====

		/// \brief Wrapped RapidJSON getter.
		/// Example syntax for depth 2, asking for a float:  
		/// \code {.cpp}
		/// ParserObject.getFloat<parser::base, parser::child>()
		/// \endcode
		/// @tparam PARENTKEY The JSON obj where to search.
		/// @tparam CHILDKEY The key to search for.
		/// @returns This returns a float value or throws if that's not possible.
		template <typename PARENTKEY, typename CHILDKEY>
		float getFloat() const
		{
			checkKeys<PARENTKEY, CHILDKEY>();
			if (!parse<PARENTKEY, CHILDKEY>().IsNumber())
				throw CException{ "Value of \"" + static_cast<std::string>(PARENTKEY::Key) + "\":\"" + static_cast<std::string>(CHILDKEY::Key) + "\" is no number.", INFO };

			return parse<PARENTKEY, CHILDKEY>().GetFloat();
		}
		/// \brief Wrapped RapidJSON getter.
		/// Example syntax for depth 2, asking for a float:  
		/// \code {.cpp}
		/// ParserObject.getFloat<parser::base, parser::child>()
		/// \endcode
		/// @tparam PARENTKEY The JSON obj where to search.
		/// @tparam CHILDKEY The key to search for.
		/// @returns This returns an int value or throws if that's not possible.
		template <typename PARENTKEY, typename CHILDKEY>
		int getInt() const
		{
			checkKeys<PARENTKEY, CHILDKEY>();
			if (!parse<PARENTKEY, CHILDKEY>().IsNumber())
				throw CException{ "Value of \"" + static_cast<std::string>(PARENTKEY::Key) + "\":\"" + static_cast<std::string>(CHILDKEY::Key) + "\" is no number.", INFO };

			return parse<PARENTKEY, CHILDKEY>().GetInt();
		}
		/// \brief Wrapped RapidJSON getter.
		/// Example syntax for depth 2, asking for a float:  
		/// \code {.cpp}
		/// ParserObject.getFloat<parser::base, parser::child>()
		/// \endcode
		/// @tparam PARENTKEY The JSON obj where to search.
		/// @tparam CHILDKEY The key to search for.
		/// @returns This returns a string or throws if that's not possible.
		template <typename PARENTKEY, typename CHILDKEY>
		std::string getString() const
		{
			checkKeys<PARENTKEY, CHILDKEY>();
			if (!parse<PARENTKEY, CHILDKEY>().IsString())
				throw CException{ "Value of \"" + static_cast<std::string>(PARENTKEY::Key) + "\":\"" + static_cast<std::string>(CHILDKEY::Key) + "\" is no string.", INFO };

			return parse<PARENTKEY, CHILDKEY>().GetString();
		}
		/// \brief Wrapped RapidJSON getter.
		/// Example syntax for depth 2, asking for a float:  
		/// \code {.cpp}
		/// ParserObject.getFloat<parser::base, parser::child>()
		/// \endcode
		/// @tparam PARENTKEY The JSON obj where to search.
		/// @tparam CHILDKEY The key to search for.
		/// @returns This returns a boolean or throws if that's not possible.
		template <typename PARENTKEY, typename CHILDKEY>
		bool getBool() const
		{
			checkKeys<PARENTKEY, CHILDKEY>();
			if (!parse<PARENTKEY, CHILDKEY>().IsBool())
				throw CException{ "Value of \"" + static_cast<std::string>(PARENTKEY::Key) + "\":\"" + static_cast<std::string>(CHILDKEY::Key) + "\" is no boolean.", INFO };

			return parse<PARENTKEY, CHILDKEY>().GetBool();
		}
		/// \brief Wrapped RapidJSON getter for an array.
		/// Example syntax for depth 2, asking for a float:  
		/// \code {.cpp}
		/// ParserObject.getFloat<parser::base, parser::child>()
		/// \endcode
		/// @tparam PARENTKEY The JSON obj where to search.
		/// @tparam CHILDKEY The key to search for.
		/// @returns This returns a vector of floats or throws if that's not possible.
		template <typename PARENTKEY, typename CHILDKEY>
		std::vector<float> getVFloat() const
		{
			checkKeys<PARENTKEY, CHILDKEY>();
			const auto& value{ parse<PARENTKEY, CHILDKEY>() };
			if (!value.IsArray())
				throw CException{ "Value of \"" + static_cast<std::string>(PARENTKEY::Key) + "\":\"" + static_cast<std::string>(CHILDKEY::Key) + "\" is no array.", INFO };

			std::vector<float> vec{};
			for (rapidjson::SizeType i{ 0 }; i < value.Size(); ++i)
			{
				if (!value[i].IsNumber())
					throw CException{ "The " + std::to_string(i) + " value of \"" + static_cast<std::string>(PARENTKEY::Key) + "\":\"" + static_cast<std::string>(CHILDKEY::Key) + "\" is no number.", INFO };
				else
					vec.push_back(value[i].GetFloat());
			}
			return vec;
		}
		/// \brief Wrapped RapidJSON getter for an array.
		/// Example syntax for depth 2, asking for a float:  
		/// \code {.cpp}
		/// ParserObject.getFloat<parser::base, parser::child>()
		/// \endcode
		/// @tparam PARENTKEY The JSON obj where to search.
		/// @tparam CHILDKEY The key to search for.
		/// @returns This returns a vector of ints or throws if that's not possible.
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
					throw CException{ "The " + std::to_string(i) + " value of \"" + static_cast<std::string>(PARENTKEY::Key) + "\":\"" + static_cast<std::string>(CHILDKEY::Key) + "\" is no number.", INFO };
				else
					vec.push_back(value[i].GetInt());
			}
			return vec;
		}
		/// \brief Wrapped RapidJSON getter for an array.
		/// Example syntax for depth 2, asking for a float:  
		/// \code {.cpp}
		/// ParserObject.getFloat<parser::base, parser::child>()
		/// \endcode
		/// @tparam PARENTKEY The JSON obj where to search.
		/// @tparam CHILDKEY The key to search for.
		/// @returns This returns a vector of strings or throws if that's not possible.
		template <typename PARENTKEY, typename CHILDKEY>
		std::vector<std::string> getVString() const
		{
			checkKeys<PARENTKEY, CHILDKEY>();
			const auto& value{ parse<PARENTKEY, CHILDKEY>() };
			if (!value.IsArray())
				throw CException{ "Value of \"" + static_cast<std::string>(PARENTKEY::Key) + "\":\"" + static_cast<std::string>(CHILDKEY::Key) + "\" is no array.", INFO };

			std::vector<std::string> vec{};
			for (rapidjson::SizeType i{ 0 }; i < value.Size(); ++i)
			{
				if (!value[i].IsString())
					throw CException{ "The " + std::to_string(i) + " value of \"" + static_cast<std::string>(PARENTKEY::Key) + "\":\"" + static_cast<std::string>(CHILDKEY::Key) + "\" is no string.", INFO };
				else
					vec.push_back(value[i].GetString());
			}
			return vec;
		}
		/// \brief Wrapped RapidJSON getter for an array of arrays.
		/// Example syntax for depth 2, asking for a float:  
		/// \code {.cpp}
		/// ParserObject.getFloat<parser::base, parser::child>()
		/// \endcode
		/// @tparam PARENTKEY The JSON obj where to search.
		/// @tparam CHILDKEY The key to search for.
		/// @returns This returns a vector of vector of ints or throws if that's not possible.
		template <typename PARENTKEY, typename CHILDKEY>
		std::vector<std::vector<int>> getVVInt() const
		{
			checkKeys<PARENTKEY, CHILDKEY>();
			const auto& value{ parse<PARENTKEY, CHILDKEY>() };
			if (!value.IsArray())
				throw CException{ "Value of \"" + static_cast<std::string>(PARENTKEY::Key) + "\":\"" + static_cast<std::string>(CHILDKEY::Key) + "\" is no array.", INFO };

			std::vector<std::vector<int>> vec{};
			for (rapidjson::SizeType i{ 0 }; i < value.Size(); ++i)
			{
				if (!value[i].IsArray())
					throw CException{ "The " + std::to_string(i) + " value of \"" + static_cast<std::string>(PARENTKEY::Key) + "\":\"" + static_cast<std::string>(CHILDKEY::Key) + "\" is no array.", INFO };
				else
				{
					vec.resize(vec.size() + 1);
					for (rapidjson::SizeType j{ 0 }; j < value[i].Size(); ++j)
						if (!value[i][j].IsNumber())
							throw CException{ "The " + std::to_string(j) + " value of \"" + std::to_string(i) + " value of \"" + static_cast<std::string>(PARENTKEY::Key) + "\":\"" + static_cast<std::string>(CHILDKEY::Key) + "\" is no number.", INFO };
						else
							vec[i].push_back(value[i][j].GetInt());
				}
			}
			return vec;
		}
		/// \brief Wrapped RapidJSON getter for a 2D array.
		/// Example syntax for depth 2, asking for a float:  
		/// \code {.cpp}
		/// ParserObject.getFloat<parser::base, parser::child>()
		/// \endcode
		/// @tparam PARENTKEY The JSON obj where to search.
		/// @tparam CHILDKEY The key to search for.
		/// @returns This returns a 2D vector.
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
					throw CException{ "The " + std::to_string(i) + " value of \"" + static_cast<std::string>(PARENTKEY::Key) + "\":\"" + static_cast<std::string>(CHILDKEY::Key) + "\" is no number.", INFO };
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

		//===== depth 3 =====

		/// \brief Wrapped RapidJSON getter.
		/// Example syntax for depth 3, asking for a float:  
		/// \code {.cpp}
		/// ParserObject.getFloat<parser::base, parser::child, parser::grandchild>()
		/// \endcode
		/// @tparam PARENTKEY The JSON obj where to search.
		/// @tparam CHILDKEY The JSON obj inside of the Parent obj where to search.
		/// @tparam GRANDCHILDKEY The key to search for.
		/// @returns This returns a float value or throws if that's not possible.
		template <typename PARENTKEY, typename CHILDKEY, typename GRANDCHILDKEY>
		float getFloat() const
		{
			checkKeys<PARENTKEY, CHILDKEY, GRANDCHILDKEY>();
			if (!parse<PARENTKEY, CHILDKEY, GRANDCHILDKEY>().IsNumber())
				throw CException{ "Value of \"" + static_cast<std::string>(PARENTKEY::Key) + "\":\"" + static_cast<std::string>(CHILDKEY::Key) + "\":\"" + static_cast<std::string>(GRANDCHILDKEY::Key) + "\" is no number.", INFO };

			return parse<PARENTKEY, CHILDKEY, GRANDCHILDKEY>().GetFloat();
		}
		/// \brief Wrapped RapidJSON getter.
		/// Example syntax for depth 3, asking for a float:  
		/// \code {.cpp}
		/// ParserObject.getFloat<parser::base, parser::child, parser::grandchild>()
		/// \endcode
		/// @tparam PARENTKEY The JSON obj where to search.
		/// @tparam CHILDKEY The JSON obj inside of the Parent obj where to search.
		/// @tparam GRANDCHILDKEY The key to search for.
		/// @returns This returns an int value or throws if that's not possible.
		template <typename PARENTKEY, typename CHILDKEY, typename GRANDCHILDKEY>
		int getInt() const
		{
			checkKeys<PARENTKEY, CHILDKEY, GRANDCHILDKEY>();
			if (!parse<PARENTKEY, CHILDKEY, GRANDCHILDKEY>().IsNumber())
				throw CException{ "Value of \"" + static_cast<std::string>(PARENTKEY::Key) + "\":\"" + static_cast<std::string>(CHILDKEY::Key) + "\":\"" + static_cast<std::string>(GRANDCHILDKEY::Key) + "\" is no number.", INFO };

			return parse<PARENTKEY, CHILDKEY, GRANDCHILDKEY>().GetInt();
		}
		/// \brief Wrapped RapidJSON getter.
		/// Example syntax for depth 3, asking for a float:  
		/// \code {.cpp}
		/// ParserObject.getFloat<parser::base, parser::child, parser::grandchild>()
		/// \endcode
		/// @tparam PARENTKEY The JSON obj where to search.
		/// @tparam CHILDKEY The JSON obj inside of the Parent obj where to search.
		/// @tparam GRANDCHILDKEY The key to search for.
		/// @returns This returns a string or throws if that's not possible.
		template <typename PARENTKEY, typename CHILDKEY, typename GRANDCHILDKEY>
		std::string getString() const
		{
			checkKeys<PARENTKEY, CHILDKEY, GRANDCHILDKEY>();
			if (!parse<PARENTKEY, CHILDKEY, GRANDCHILDKEY>().IsString())
				throw CException{ "Value of \"" + static_cast<std::string>(PARENTKEY::Key) + "\":\"" + static_cast<std::string>(CHILDKEY::Key) + "\":\"" + static_cast<std::string>(GRANDCHILDKEY::Key) + "\" is no string.", INFO };

			return parse<PARENTKEY, CHILDKEY, GRANDCHILDKEY>().GetString();
		}
		/// \brief Wrapped RapidJSON getter.
		/// Example syntax for depth 3, asking for a float:  
		/// \code {.cpp}
		/// ParserObject.getFloat<parser::base, parser::child, parser::grandchild>()
		/// \endcode
		/// @tparam PARENTKEY The JSON obj where to search.
		/// @tparam CHILDKEY The JSON obj inside of the Parent obj where to search.
		/// @tparam GRANDCHILDKEY The key to search for.
		/// @returns This returns a boolean or throws if that's not possible.
		template <typename PARENTKEY, typename CHILDKEY, typename GRANDCHILDKEY>
		bool getBool() const
		{
			checkKeys<PARENTKEY, CHILDKEY, GRANDCHILDKEY>();
			if (!parse<PARENTKEY, CHILDKEY, GRANDCHILDKEY>().IsBool())
				throw CException{ "Value of \"" + static_cast<std::string>(PARENTKEY::Key) + "\":\"" + static_cast<std::string>(CHILDKEY::Key) + "\":\"" + static_cast<std::string>(GRANDCHILDKEY::Key) + "\" is no boolean.", INFO };

			return parse<PARENTKEY, CHILDKEY, GRANDCHILDKEY>().GetBool();
		}
		/// \brief Wrapped RapidJSON getter for an array.
		/// Example syntax for depth 3, asking for a float:  
		/// \code {.cpp}
		/// ParserObject.getFloat<parser::base, parser::child, parser::grandchild>()
		/// \endcode
		/// @tparam PARENTKEY The JSON obj where to search.
		/// @tparam CHILDKEY The JSON obj inside of the Parent obj where to search.
		/// @tparam GRANDCHILDKEY The key to search for.
		/// @returns This returns a vector of floats or throws if that's not possible.
		template <typename PARENTKEY, typename CHILDKEY, typename GRANDCHILDKEY>
		std::vector<float> getVFloat() const
		{
			checkKeys<PARENTKEY, CHILDKEY, GRANDCHILDKEY>();
			const auto& value{ parse<PARENTKEY, CHILDKEY, GRANDCHILDKEY>() };
			if (!value.IsArray())
				throw CException{ "Value of \"" + static_cast<std::string>(PARENTKEY::Key) + "\":\"" + static_cast<std::string>(CHILDKEY::Key) + "\":\"" + static_cast<std::string>(GRANDCHILDKEY::Key) + "\" is no array.", INFO };

			std::vector<float> vec{};
			for (rapidjson::SizeType i{ 0 }; i < value.Size(); ++i)
			{
				if (!value[i].IsNumber())
					throw CException{ "The " + std::to_string(i) + " value of \"" + static_cast<std::string>(PARENTKEY::Key) + "\":\"" + static_cast<std::string>(CHILDKEY::Key) + "\":\"" + static_cast<std::string>(GRANDCHILDKEY::Key) + "\" is no number.", INFO };
				else
					vec.push_back(value[i].GetFloat());
			}
			return vec;
		}
		/// \brief Wrapped RapidJSON getter for an array.
		/// Example syntax for depth 3, asking for a float:  
		/// \code {.cpp}
		/// ParserObject.getFloat<parser::base, parser::child, parser::grandchild>()
		/// \endcode
		/// @tparam PARENTKEY The JSON obj where to search.
		/// @tparam CHILDKEY The JSON obj inside of the Parent obj where to search.
		/// @tparam GRANDCHILDKEY The key to search for.
		/// @returns This returns a vector of ints or throws if that's not possible.
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
					throw CException{ "The " + std::to_string(i) + " value of \"" + static_cast<std::string>(PARENTKEY::Key) + "\":\"" + static_cast<std::string>(CHILDKEY::Key) + "\":\"" + static_cast<std::string>(GRANDCHILDKEY::Key) + "\" is no number.", INFO };
				else
					vec.push_back(value[i].GetInt());
			}
			return vec;
		}
		/// \brief Wrapped RapidJSON getter for an array.
		/// Example syntax for depth 3, asking for a float:  
		/// \code {.cpp}
		/// ParserObject.getFloat<parser::base, parser::child, parser::grandchild>()
		/// \endcode
		/// @tparam PARENTKEY The JSON obj where to search.
		/// @tparam CHILDKEY The JSON obj inside of the Parent obj where to search.
		/// @tparam GRANDCHILDKEY The key to search for.
		/// @returns This returns a vector of strings or throws if that's not possible.
		template <typename PARENTKEY, typename CHILDKEY, typename GRANDCHILDKEY>
		std::vector<std::string> getVString() const
		{
			checkKeys<PARENTKEY, CHILDKEY, GRANDCHILDKEY>();
			const auto& value{ parse<PARENTKEY, CHILDKEY, GRANDCHILDKEY>() };
			if (!value.IsArray())
				throw CException{ "Value of \"" + static_cast<std::string>(PARENTKEY::Key) + "\":\"" + static_cast<std::string>(CHILDKEY::Key) + "\":\"" + static_cast<std::string>(GRANDCHILDKEY::Key) + "\" is no array.", INFO };

			std::vector<std::string> vec{};
			for (rapidjson::SizeType i{ 0 }; i < value.Size(); ++i)
			{
				if (!value[i].IsString())
					throw CException{ "The " + std::to_string(i) + " value of \"" + static_cast<std::string>(PARENTKEY::Key) + "\":\"" + static_cast<std::string>(CHILDKEY::Key) + "\":\"" + static_cast<std::string>(GRANDCHILDKEY::Key) + "\" is no string.", INFO };
				else
					vec.push_back(value[i].GetString());
			}
			return vec;
		}
		/// \brief Wrapped RapidJSON getter for an array of arrays.
		/// Example syntax for depth 3, asking for a float:  
		/// \code {.cpp}
		/// ParserObject.getFloat<parser::base, parser::child, parser::grandchild>()
		/// \endcode
		/// @tparam PARENTKEY The JSON obj where to search.
		/// @tparam CHILDKEY The JSON obj inside of the Parent obj where to search.
		/// @tparam GRANDCHILDKEY The key to search for.
		/// @returns This returns a vector of vector of ints or throws if that's not possible.
		template <typename PARENTKEY, typename CHILDKEY, typename GRANDCHILDKEY>
		std::vector<std::vector<int>> getVVInt() const
		{
			checkKeys<PARENTKEY, CHILDKEY, GRANDCHILDKEY>();
			const auto& value{ parse<PARENTKEY, CHILDKEY, GRANDCHILDKEY>() };
			if (!value.IsArray())
				throw CException{ "Value of \"" + static_cast<std::string>(PARENTKEY::Key) + "\":\"" + static_cast<std::string>(CHILDKEY::Key) + "\":\"" + static_cast<std::string>(GRANDCHILDKEY::Key) + "\" is no array.", INFO };

			std::vector<std::vector<int>> vec{};
			for (rapidjson::SizeType i{ 0 }; i < value.Size(); ++i)
			{
				if (!value[i].IsArray())
					throw CException{ "The " + std::to_string(i) + " value of \"" + static_cast<std::string>(PARENTKEY::Key) + "\":\"" + static_cast<std::string>(CHILDKEY::Key) + "\":\"" + static_cast<std::string>(GRANDCHILDKEY::Key) + "\" is no array.", INFO };
				else
				{
					vec.resize(vec.size() + 1);
					for (rapidjson::SizeType j{ 0 }; j < value[i].Size(); ++j)
						if (!value[i][j].IsNumber())
							throw CException{ "The " + std::to_string(j) + " value of \"" + std::to_string(i) + " value of \"" + static_cast<std::string>(PARENTKEY::Key) + "\":\"" + static_cast<std::string>(CHILDKEY::Key) + "\":\"" + static_cast<std::string>(GRANDCHILDKEY::Key) + "\" is no number.", INFO };
						else
							vec[i].push_back(value[i][j].GetInt());
				}
			}
			return vec;
		}
		/// \brief Wrapped RapidJSON getter for a 2D array.
		/// Example syntax for depth 3, asking for a float:  
		/// \code {.cpp}
		/// ParserObject.getFloat<parser::base, parser::child, parser::grandchild>()
		/// \endcode
		/// @tparam PARENTKEY The JSON obj where to search.
		/// @tparam CHILDKEY The JSON obj inside of the Parent obj where to search.
		/// @tparam GRANDCHILDKEY The key to search for.
		/// @returns This returns a 2D vector or throws if that's not possible.
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
					throw CException{ "The " + std::to_string(i) + " value of \"" + static_cast<std::string>(PARENTKEY::Key) + "\":\"" + static_cast<std::string>(CHILDKEY::Key) + "\":\"" + static_cast<std::string>(GRANDCHILDKEY::Key) + "\" is no number.", INFO };
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

		/// \brief Helper fct to determine color.
		/// @param color The color string which gets translated to olc color.
		/// @returns The corresponding olc color.
		olc::Pixel color(std::string color) const;

	private:
		////===== helper templates =====

		/// \brief Helper fct for direct JSON parsing.
		/// This parses in the JSON document for a specific key.
		/// @tparam KEY The key to search for.
		/// @returns This returns a RapidJSON value.
		template <typename KEY>
		const rapidjson::Value& parse() const
		{
			return m_settings[KEY::Key];
		}
		/// \brief Helper fct for direct JSON parsing.
		/// This parses in the JSON document for a specific key.
		/// @tparam PARENTKEY The parent object in which to search for.
		/// @tparam CHILDKEY The key to search for.
		/// @returns This returns a RapidJSON value.
		template <typename PARENTKEY, typename CHILDKEY>
		const rapidjson::Value& parse() const
		{
			return m_settings[PARENTKEY::Key].GetObj()[CHILDKEY::Key];
		}
		/// \brief Helper fct for direct JSON parsing.
		/// This parses in the JSON document for a specific key.
		/// @tparam PARENTKEY The parent object in which to search for the child obj.
		/// @tparam CHILDKEY The child object in which to search for.
		/// @tparam GRANDCHILDKEY The key to search for.
		/// @returns This returns a RapidJSON value.
		template <typename PARENTKEY, typename CHILDKEY, typename GRANDCHILDKEY>
		const rapidjson::Value& parse() const
		{
			return m_settings[PARENTKEY::Key].GetObj()[CHILDKEY::Key].GetObj()[GRANDCHILDKEY::Key];
		}
		/// \brief Helper fct to check for validity.
		/// Throws if there are errors found in a specific object.
		/// @tparam PARENTKEY The parent object which should be existent.
		/// @tparam CHILDKEY The key which should be existent.
		template <typename PARENTKEY, typename CHILDKEY>
		void checkKeys() const
		{
			if (!m_settings.HasMember(PARENTKEY::Key))
				throw CException{ "Wrong syntax or can't find key for object \"" + static_cast<std::string>(PARENTKEY::Key) + "\" in settings file.", INFO };
			if (!parse<PARENTKEY>().HasMember(CHILDKEY::Key))
				throw CException{ "Wrong syntax or can't find key for object \"" + static_cast<std::string>(PARENTKEY::Key) + "\":\"" + static_cast<std::string>(CHILDKEY::Key) + "\" in settings file.", INFO };
		}
		/// \brief Helper fct to check for validity.
		/// Throws if there are errors found in a specific object.
		/// @tparam PARENTKEY The parent object which should be existent.
		/// @tparam CHILDKEY The child object which should be existent.
		/// @tparam GRANDCHILDKEY The key which should be existent.
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


		/// \brief Execution path of the application.
		std::filesystem::path m_SettingsPath;

		/// \brief The RapidJSON object where all settings are being parsed to
		rapidjson::Document m_settings;

		/// \brief Start of file read in.
		void start();
		/// \brief Read all settings files in and create the rapidjson::Document
		void readFiles();
		/// \brief Merger, if there are more than one settings file.
		void merge(rapidjson::Document& settings, rapidjson::Document& document);
	};

} //end of namespace bleep