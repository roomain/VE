#pragma once
/***********************************************
* @headerfile VulkanContext.h
* @date 25 / 08 / 2024
* @author Roomain
************************************************/
#include <vector>
#include <ranges>
#include <algorithm>

template<typename DataType, typename SearchType,  typename Predicate>
bool contains(const std::vector<DataType>& a_data, const std::vector<SearchType>& a_search, const Predicate& a_predicate)
{
    for(const auto& search : a_search)
    {
        if(!std::ranges::any_of(a_data, [&search, &a_predicate](const auto& a_value){return a_predicate(search, a_value);} ))
            return false;
    }
    return true;
}

template<typename CompareType, typename SearchType, typename Predicate>
bool contains(typename const std::vector<CompareType>::const_iterator& a_from, typename const std::vector<CompareType>::const_iterator& a_to, const std::vector<SearchType>& a_search, const Predicate& a_predicate)
{
    for (const auto& search : a_search)
    {
        if (!std::ranges::any_of(a_from, a_to, [&search, &a_predicate](const auto& a_value) {return a_predicate(search, a_value); }))
            return false;
    }
    return true;
}

template<typename DataType, typename SearchType>
bool contains(const std::vector<DataType>& a_data, const std::vector<SearchType>& a_search)
{
    for(const auto& search : a_search)
    {
        if(!std::ranges::any_of(a_data, search))
            return false;
    }
    return true;
}
