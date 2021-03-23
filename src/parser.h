//==============================================================================
/// \copyright (c) 2021, Benedikt Michael.
//==============================================================================
/// \file parser.h
/// \brief Loads & checks the settings.
//==============================================================================

#include "node.h"

#include <filesystem>
#include <vector>


class CParser
{
public:
	CParser();

	//to find a specific root node
	std::shared_ptr<CNode> find(std::string name) const;

private:
	//root elements of the xml files
	std::vector<std::shared_ptr<CNode>> mRoots;

	//settings path
	std::filesystem::path mSettingsPath;
	//container for all input strings out of the files
	std::vector<std::string> mInput;

	//read a xml files
	void readFiles();
};