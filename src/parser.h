//==============================================================================
/// 2021, Benedikt Michael.
//==============================================================================
/// parser.h
/// Loads & checks the settings.
//==============================================================================


#include <rapidjson/document.h>

#include <filesystem>
#include <vector>


class CParser
{
public:
	CParser(std::filesystem::path& currentPath);

	//start of parsing. returns true if no exceptions are thrown.
	bool parse(rapidjson::Document& settings);

private://settings path
	std::filesystem::path m_SettingsPath;

	//read a xml files
	void readFiles(rapidjson::Document& settings);
};