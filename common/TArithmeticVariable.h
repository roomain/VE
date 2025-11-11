#pragma once
/***********************************************
* @headerfile TArithmeticVariable.h
* @date 10 / 11 / 2025
* @author Roomain
************************************************/
#include <type_traits>
#include "TVariable.h"

template<typename Type> requires std::is_arithmetic_v<Type> 
class TArithmeticVariable : public TVariable<Type>
{
public:
    TVariable<Type>::TVariable<Type>();

    template<typename OtherType> requires std::is_arithmetic_v<OtherType> 
    const TArithmeticVariable<type>& operator = (const OtherType& a_data)
    {
        m_data = a_data;
        callbacks();
        return *this;
    }

    template<typename OtherType> requires std::is_arithmetic_v<OtherType> 
    const TArithmeticVariable<type>& operator = (OtherType&& a_data)noexcept 
    {
        m_data = a_data;
        callbacks();
        return *this;
    }

    template<typename OtherType> requires std::is_arithmetic_v<OtherType> 
    const TArithmeticVariable<type>& operator = (const TVariable<OtherType>& a_other)
    {
        m_data = a_other.m_data;
        callbacks();
        return *this;
    }

    template<typename OtherType> requires std::is_arithmetic_v<OtherType> 
    const TArithmeticVariable<type>& operator = (TVariable<OtherType>&& a_other)noexcept
    {
        m_data = a_other.m_data;
        callbacks();
        return *this;
    }


    //--------------------------------------------------------------------------------------
    template<typename OtherType> requires std::is_arithmetic_v<OtherType> 
    const TArithmeticVariable<type>& operator += (const OtherType& a_data)
    {
        m_data += a_data;
        callbacks();
        return *this;
    }

    template<typename OtherType> requires std::is_arithmetic_v<OtherType> 
    const TArithmeticVariable<type>& operator += (OtherType&& a_data)noexcept 
    {
        m_data += a_data;
        callbacks();
        return *this;
    }

    template<typename OtherType> requires std::is_arithmetic_v<OtherType> 
    const TArithmeticVariable<type>& operator += (const TVariable<OtherType>& a_other)
    {
        m_data += a_other.m_data;
        callbacks();
        return *this;
    }

    template<typename OtherType> requires std::is_arithmetic_v<OtherType> 
    const TArithmeticVariable<type>& operator += (TVariable<OtherType>&& a_other)noexcept
    {
        m_data += a_other.m_data;
        callbacks();
        return *this;
    }
    //--------------------------------------------------------------------------------------
    template<typename OtherType> requires std::is_arithmetic_v<OtherType> 
    const TArithmeticVariable<type>& operator -= (const OtherType& a_data)
    {
        m_data -= a_data;
        callbacks();
        return *this;
    }

    template<typename OtherType> requires std::is_arithmetic_v<OtherType> 
    const TArithmeticVariable<type>& operator -= (OtherType&& a_data)noexcept 
    {
        m_data -= a_data;
        callbacks();
        return *this;
    }

    template<typename OtherType> requires std::is_arithmetic_v<OtherType> 
    const TArithmeticVariable<type>& operator -= (const TVariable<OtherType>& a_other)
    {
        m_data -= a_other.m_data;
        callbacks();
        return *this;
    }

    template<typename OtherType> requires std::is_arithmetic_v<OtherType> 
    const TArithmeticVariable<type>& operator -= (TVariable<OtherType>&& a_other)noexcept
    {
        m_data -= a_other.m_data;
        callbacks();
        return *this;
    }
    //--------------------------------------------------------------------------------------
    template<typename OtherType> requires std::is_arithmetic_v<OtherType> 
    const TArithmeticVariable<type>& operator *= (const OtherType& a_data)
    {
        m_data *= a_data;
        callbacks();
        return *this;
    }

    template<typename OtherType> requires std::is_arithmetic_v<OtherType> 
    const TArithmeticVariable<type>& operator *= (OtherType&& a_data)noexcept 
    {
        m_data *= a_data;
        callbacks();
        return *this;
    }

    template<typename OtherType> requires std::is_arithmetic_v<OtherType> 
    const TArithmeticVariable<type>& operator *= (const TVariable<OtherType>& a_other)
    {
        m_data *= a_other.m_data;
        callbacks();
        return *this;
    }

    template<typename OtherType> requires std::is_arithmetic_v<OtherType> 
    const TArithmeticVariable<type>& operator *= (TVariable<OtherType>&& a_other)noexcept
    {
        m_data *= a_other.m_data;
        callbacks();
        return *this;
    }
    //--------------------------------------------------------------------------------------
    template<typename OtherType> requires std::is_arithmetic_v<OtherType> 
    const TArithmeticVariable<type>& operator /= (const OtherType& a_data)
    {
        m_data += a_data;
        callbacks();
        return *this;
    }

    template<typename OtherType> requires std::is_arithmetic_v<OtherType> 
    const TArithmeticVariable<type>& operator /= (OtherType&& a_data)noexcept 
    {
        m_data += a_data;
        callbacks();
        return *this;
    }

    template<typename OtherType> requires std::is_arithmetic_v<OtherType> 
    const TArithmeticVariable<type>& operator /= (const TVariable<OtherType>& a_other)
    {
        m_data += a_other.m_data;
        callbacks();
        return *this;
    }

    template<typename OtherType> requires std::is_arithmetic_v<OtherType> 
    const TArithmeticVariable<type>& operator /= (TVariable<OtherType>&& a_other)noexcept
    {
        m_data += a_other.m_data;
        callbacks();
        return *this;
    }
    //--------------------------------------------------------------------------------------
    template<typename OtherType> requires std::is_arithmetic_v<OtherType> 
    const TArithmeticVariable<type>& operator < (const OtherType& a_data)
    {
        return m_data < a_data;
    }

    template<typename OtherType> requires std::is_arithmetic_v<OtherType> 
    const TArithmeticVariable<type>& operator < (OtherType&& a_data)noexcept 
    {
        return m_data < a_data;
    }

    template<typename OtherType> requires std::is_arithmetic_v<OtherType> 
    const TArithmeticVariable<type>& operator < (const TVariable<OtherType>& a_other)
    {
        return m_data < a_other.m_data;
    }

    template<typename OtherType> requires std::is_arithmetic_v<OtherType> 
    const TArithmeticVariable<type>& operator < (TVariable<OtherType>&& a_other)noexcept
    {
        return m_data < a_other.m_data;
    }
    //--------------------------------------------------------------------------------------
    template<typename OtherType> requires std::is_arithmetic_v<OtherType> 
    const TArithmeticVariable<type>& operator > (const OtherType& a_data)
    {
        return m_data > a_data;
    }

    template<typename OtherType> requires std::is_arithmetic_v<OtherType> 
    const TArithmeticVariable<type>& operator > (OtherType&& a_data)noexcept 
    {
        return m_data > a_data;
    }

    template<typename OtherType> requires std::is_arithmetic_v<OtherType> 
    const TArithmeticVariable<type>& operator > (const TVariable<OtherType>& a_other)
    {
        return m_data > a_other.m_data;
    }

    template<typename OtherType> requires std::is_arithmetic_v<OtherType> 
    const TArithmeticVariable<type>& operator > (TVariable<OtherType>&& a_other)noexcept
    {
        return m_data > a_other.m_data;
    }
};