//==============================================================================
/// 2021, Benedikt Michael
//==============================================================================
/// jsParser.cpp
/// Loads & checks the settings.
//==============================================================================

#include "jsParser.h"

#include <iostream>
#include <fstream>


CParser::CParser(std::filesystem::path& currentPath) :
	m_SettingsPath{ currentPath / "settings" },
	m_settings {}
{
	std::cout << std::endl << "Starting parser." << std::endl;
	start();
	std::cout << "Parser end." << std::endl << std::endl;
}

void CParser::start()
{
	try
	{
		if (!std::filesystem::exists(m_SettingsPath))
			throw CException{ m_SettingsPath.string() + " doesn't exist.", INFO };

		m_settings.SetObject();
		readFiles();
	}
	catch (CException& exception)
	{
		exception.print();
	}
	catch (...)
	{
		std::cerr << "Unhandled exception in " << S(__FILE__);
	}
}

void CParser::readFiles()
{
	for (const auto& file : std::filesystem::directory_iterator(m_SettingsPath))
	{
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

				rapidjson::Document tmpDocument;
				tmpDocument.Parse(fileData.c_str());

				std::cout << "Try to generate nodes out of:" << std::endl << fileData << std::endl;
				merge(m_settings, tmpDocument);
			}
		}
	}
}

void CParser::merge(rapidjson::Document& settings, rapidjson::Document& document)
{
	rapidjson::Document::AllocatorType ac{ settings.GetAllocator() };

	auto itr = document.MemberBegin();
	if (itr == document.MemberEnd())
		std::cout << "Warning: No nodes generated - wrong syntax or empty file." << std::endl;
	else
		std::cout << "Nodes successfully generated." << std::endl;

	for (; itr != document.MemberEnd(); ++itr)
	{
		std::cout << "Processing \"" << itr->name.GetString() << "\"." << std::endl;
		rapidjson::Value key;
		key.CopyFrom(itr->name, ac);
		rapidjson::Value value;
		value.CopyFrom(itr->value, ac);

		std::string name{ key.GetString() }; //cause AddMember moves key
		if (settings.FindMember(key) != settings.MemberEnd())
			throw CException{ "There is already an object named " + name + " in the settings.", INFO };
		else
			settings.AddMember(key, value, ac);

		if (settings.FindMember(name.c_str()) == settings.MemberEnd())
			throw CException { "Object named " + name + " coudn't be added to the settings.", INFO };
	}
}

olc::Pixel CParser::color(std::string color) const
{
	std::map<std::string, olc::Pixel>::const_iterator tableItr{ parser::colorTable.find(color) };
	if (tableItr == parser::colorTable.end())
		throw CException{ "Can't find conversion for color: " + color, INFO };
	else
		return tableItr->second;
}
