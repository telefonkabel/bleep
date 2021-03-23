//==============================================================================
/// \copyright (c) 2021, Benedikt Michael.
//==============================================================================
/// \file node.cpp
/// \brief Manages the input out of the xml settings.
//==============================================================================

#include "node.h"
#include "exception.h"

CNode::CNode(std::string name, std::string value) :
	mName(name),
	mValue(value),
	mNodes()
{}

void CNode::value(std::string value)
{
	if (mNodes.size() != 0)
		throw CException("Error in setting value. Node " + mName + " can have either child nodes or value.");

	mValue = value;
}

std::string CNode::value() const
{
	return mValue;
}

std::string CNode::name() const
{
	return mName;
}

std::shared_ptr<CNode> CNode::find(std::string name) const
{
	for (const auto& node : mNodes)
		if (node->mName == name)
			return node;

	throw CException("Node " + name + " of parent node " + mName + " cann't be found.");
}

void CNode::emplace(CNode newNode)
{
	if (mValue != "")
		throw CException("Error in emplacing Nodes. Node " + mName + " can have either child nodes or value.");

	for (const auto& node : mNodes)
		if (node->mName == newNode.mName)
			throw CException("Node " + newNode.mName + " already exists in " + mName);

	mNodes.push_back(std::make_shared<CNode>(newNode));
}