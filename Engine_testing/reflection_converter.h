#pragma once
/***********************************************
* @headerfile reflection_converter.h
* @date 01 / 08 / 2025
* @author Roomain
************************************************/
#include <type_traits>
#include <string>
#include <vector>
#include "string_tools/string_utils.h"
#include "utils/vulkan_string_to_enum.h"

template<typename Enum> requires std::is_enum_v<Enum>
int convertEnum(const std::string& a_flag)
{
	int flag = 0;
	std::vector<std::string> vValues;
	split(a_flag, '|', vValues);
	for (const auto& val : vValues)
	{
		Enum enumVal;
		to_enum(trim(val), enumVal);
		flag |= static_cast<unsigned int>(enumVal);
	}
	return flag;
}
