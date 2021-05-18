//==============================================================================
/// 2021, Benedikt Michael.
//==============================================================================
/// parser.cpp
/// Loads & checks the settings.
//==============================================================================

#include "parser.h"
#include "exception.h"

#include <iostream>
#include <fstream>


CParser::CParser(std::filesystem::path& currentPath) :
	m_SettingsPath(currentPath / "settings")
{
}

bool CParser::parse(rapidjson::Document& settings)
{
	try
	{
		if (!std::filesystem::exists(m_SettingsPath))
			throw CException(m_SettingsPath.string() + " doesn't exist.");

		readFiles(settings);

		assert(settings.HasMember("test"));

		return true;
	}
	catch (CException& exception)
	{
		std::cerr << "Exception in parser: " << exception.msg();
		return false;
	}
	catch (...)
	{
		std::cerr << "Unhandled exception in parser.";
		return false;
	}
}

void CParser::readFiles(rapidjson::Document& settings)
{
	for (const auto& file : std::filesystem::directory_iterator(m_SettingsPath))
		if (file.path().extension() != ".json")
			throw CException("There are non-json files in the settings directory.");
		else
		{
			std::ifstream inFile(file.path());
			if (!inFile.is_open())
				throw CException("Can't open " + file.path().string());
			else
			{
				std::string fileData( (std::istreambuf_iterator<char>(inFile)), std::istreambuf_iterator<char>());
				if (fileData.empty())
					throw CException(file.path().string() + "hasn't generated any content.");

				settings.Parse(fileData.c_str());

				rapidjson::Document newSetting;
				const char testing[] = "{ \"hello\" : \"world\", \"t\" : true}";
				newSetting.Parse(testing);
				settings.AddMember("test", newSetting, settings.GetAllocator());
			}
		}
}
