#pragma once
/***********************************************
* @headerfile TVariable.h
* @date 10 / 11 / 2025
* @author Roomain
************************************************/
#include <vector>
#include <functional>

template<typename Type>
using Affect = std::function<void(const Type&a_newValue)>;

template<typename Type>
class TVariable
{
private:
    Type m_data;                        /*!< encapsulate data*/
    std::set<Affect<Type>> m_callback;  /*!< affectation callback*/

    void callbacks()const
    {
        for(const auto& cb : m_callbacks )
            cb(m_data);
    }

public:
    TVariable() = default;
    TVariable(const Type& a_data) : m_data{ a_data }{}
    TVariable(Type&& a_data)noexcept : m_data{ a_data }{}
    TVariable(const TVariable<Type>& a_other): m_data{ a_other.m_data }{}
    TVariable(TVariable<Type>&& a_other)noexcept: m_data{ strd::move(a_other.m_data) }{}

    const Type& operator()()const
    {
        returm m_data;
    }

    const Variable<type>& operator = (const Type& a_data)
    {
        m_data = a_data;
        callbacks();
        return *this;
    }

    const TVariable<type>& operator = (Type&& a_data)noexcept 
    {
        m_data = a_data;
        callbacks();
        return *this;
    }

    const TVariable<type>& operator = (const TVariable<Type>& a_other)
    {
        m_data = a_other.m_data;
        callbacks();
        return *this;
    }

    const TVariable<type>& operator = (TVariable<Type>&& a_other)noexcept
    {
        m_data = a_other.m_data;
        callbacks();
        return *this;
    }

};