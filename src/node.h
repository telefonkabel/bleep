//==============================================================================
/// \copyright (c) 2021, Benedikt Michael.
//==============================================================================
/// \file node.h
/// \brief Manages the input out of the xml settings.
//==============================================================================

#include <memory>
#include <string>
#include <vector>

class CNode
{
public:
	CNode(std::string name, std::string value = "");
	
	//to find a specific node
	std::shared_ptr<CNode> find(std::string name) const;

	//get&set value
	void value(std::string value);
	std::string value() const;

	//get name
	std::string name() const;

	//add node
	void emplace(CNode node);

private:
	//all slave nodes of the node
	std::vector<std::shared_ptr<CNode>> mNodes;

	//its name
	std::string mName;

	//its value
	std::string mValue;
};