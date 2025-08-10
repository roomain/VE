#pragma once
/***********************************************
* @headerfile bitOperators.h
* @date 16 / 08 / 2024
* @author Roomain
************************************************/
#include <type_traits>

template<typename Type>
constexpr Type operator | (const Type a_first, const Type a_second) requires std::is_enum_v<Type>
{
    return static_cast<Type>(static_cast<const unsigned int>(a_first) | static_cast<const unsigned int>(a_second));
}

template<typename Type>
constexpr Type operator & (const Type a_first, const Type a_second) requires std::is_enum_v<Type>
{
    return static_cast<Type>(static_cast<const unsigned int>(a_first) & static_cast<const unsigned int>(a_second));
}

template<typename Type>
constexpr Type operator ~ (const Type a_first) requires std::is_enum_v<Type>
{
    return static_cast<Type>(~static_cast<const unsigned int>(a_first));
}