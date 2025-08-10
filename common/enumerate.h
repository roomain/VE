#pragma once
/***********************************************
* @headerfile enumerate.h
* @date 25 / 08 / 2024
* @author Roomain
************************************************/
#include <vector>

/*@brief tool for vulkan enumeration*/
template<typename ...Arg, typename Fun, typename Type>
void enumerate(const Fun& a_fun, std::vector<Type>& a_list, Arg... a_arg)
{
    uint32_t num = 0;
    a_fun(a_arg..., &num, nullptr);
    a_list.resize(num);
    a_fun(a_arg..., &num, a_list.data());
}

template<typename ...Arg, typename Fun, typename Type, typename TypeRet>
TypeRet enumerateEx(const Fun& a_fun, std::vector<Type>& a_list, const TypeRet a_expected, Arg... a_arg)
{
    uint32_t num = 0;
    TypeRet result = a_fun(a_arg..., &num, nullptr);
    if (result == a_expected)
    {
        a_list.resize(num);
        return a_fun(a_arg..., &num, a_list.data());
    }
    return result;
}

