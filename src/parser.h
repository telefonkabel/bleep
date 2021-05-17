//==============================================================================
/// \copyright (c) 2021, Benedikt Michael.
//==============================================================================
/// \file parser.h
/// \brief Loads & checks the settings.
//==============================================================================


#include <rapidjson/document.h>

#include <filesystem>
#include <vector>


class CParser
{
public:
	CParser();

	//start of parsing. returns true if no exceptions are thrown.
	bool parse(rapidjson::Document& settings);

private://settings path
	std::filesystem::path mSettingsPath;

	//read a xml files
	void readFiles(rapidjson::Document& settings);
};