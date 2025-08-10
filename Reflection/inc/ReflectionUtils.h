#pragma once
#include <boost/json.hpp>


inline bool getCurrentProfile(const boost::json::value& a_node, const std::string& a_profile, std::string& a_parent)
{
	bool isProfile = false;
	std::error_code code;
	if (auto nameNode = a_node.find_pointer("name", code))
	{
		isProfile = nameNode->get_string() == a_profile;
		if (isProfile)
		{
			if (const auto parentNode = nameNode->find_pointer("parent", code))
				a_parent = parentNode->get_string();
		}
	}
	else
	{
		//code.message();
		return false;
	}
	return isProfile;
}

inline void getIncludeFile(const boost::json::value& a_node, std::vector<std::string>& a_includeFiles)
{
	if (auto array = a_node.if_array())
	{
		for (const auto& node : *array)
		{
			a_includeFiles.emplace_back(node.get_string());
		}
	}
}
