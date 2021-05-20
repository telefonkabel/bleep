//==============================================================================
/// 2021, Benedikt Michael
//==============================================================================
/// parser.cpp
/// Loads & checks the settings.
//==============================================================================

#include "parser.h"
#include "exception.h"

#include <iostream>
#include <fstream>


CParser::CParser(std::filesystem::path& currentPath) :
	m_SettingsPath{ currentPath / "settings" }
{
}

void CParser::parse(rapidjson::Document& settings)
{
	try
	{
		if (!std::filesystem::exists(m_SettingsPath))
			throw CException{ m_SettingsPath.string() + " doesn't exist.", INFO };

		readFiles(settings);
	}
	catch (CException& exception)
	{
		std::cerr << "Error while parsing - defaults will be used";
		std::cerr << exception.msg();
	}
	catch (...)
	{
		std::cerr << "Error while parsing - defaults will be used";
		std::cerr << "Unhandled exception in parser.";
	}
}

void CParser::readFiles(rapidjson::Document& settings)
{
	for (const auto& file : std::filesystem::directory_iterator(m_SettingsPath))
		if (file.path().extension() != ".json")
			throw CException{ "There are non-json files in the settings directory.", INFO };
		else
		{
			std::ifstream inFile{ file.path() };
			if (!inFile.is_open())
				throw CException{ "Can't open " + file.path().string(), INFO };
			else
			{
				std::string fileData{ std::istreambuf_iterator<char>{ inFile }, std::istreambuf_iterator<char>{} };
				if (fileData.empty())
					throw CException{ file.path().string() + "hasn't generated any content.", INFO };

				settings.Parse(fileData.c_str());

				//test
				rapidjson::Document newSetting{};
				const char testing[]{ "{ \"hello\" : \"world\", \"t\" : true}" };
				newSetting.Parse(testing);
				settings.AddMember("test", newSetting, settings.GetAllocator());

				assert(settings.HasMember("test"));
			}
		}
}
