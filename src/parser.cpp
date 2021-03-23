//==============================================================================
/// \copyright (c) 2021, Benedikt Michael.
//==============================================================================
/// \file parser.cpp
/// \brief Loads & checks the settings.
//==============================================================================

#include "parser.h"
#include "exception.h"

#include <iostream>
#include <fstream>


CParser::CParser() :
	mSettingsPath(),
	mRoots(),
	mInput()
{
	try
	{
		mSettingsPath = std::filesystem::current_path().parent_path() / "settings";
		if (!std::filesystem::exists(mSettingsPath))
			throw CException(mSettingsPath.string() + " doesn't exist.");
		else
			readFiles();
	}
	catch (CException& exception)
	{
		std::cerr << "Exception in parser: " << exception.msg();
	}
	catch (...)
	{
		std::cerr << "Unhandled exception in parser.";
	}
}

std::shared_ptr<CNode> CParser::find(std::string name) const
{
	for (const auto& node : mRoots)
		if (node->name() == name)
			return node;

	throw CException("Rootnode " + name + "can't be found.");
}

void CParser::readFiles()
{
	for (const auto& file : std::filesystem::directory_iterator(mSettingsPath))
		if (file.path().extension() != ".xml")
			throw CException("There are non-xml files in the settings directory.");
		else
		{
			std::ifstream inFile(file.path());
			if (!inFile.is_open())
				throw CException("Can't open " + file.path().string());
			else
			{
				std::string contents((std::istreambuf_iterator<char>(inFile)),
					std::istreambuf_iterator<char>());
				mInput.push_back(contents);
			}
		}
}
