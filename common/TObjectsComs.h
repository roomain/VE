#pragma once
/***********************************************
* @headerfile TObjectsComs.h
* @date  27 / 12 / 2025
* @author Roomain
************************************************/
#include <functional>
#include <unordered_map>
#include <set>
#include <boost/signals2.hpp>
#include "notCopiable.h"

/*@brief boost signal template arguments*/
template<typename ...Args>
using TSignal = boost::signals2::signal<void(Args...)>;

template<typename ...Args>
using TCallback = std::function<void(Args...)>;

template<typename ...Args>
class TCallable
{
    uint32_t m_handleGenerator = 0;
    std::unordered_map<uint32_t, TCallback<Args...>> m_callbacks;

public:
    NOT_COPIABLE(TCallable)

    using Intern_Callback = TCallback<Args...>;
    using iterator = std::vector<Intern_Callback>::iterator;
    
    uint32_t bind(const Intern_Callback& a_callback)
    {
        m_callbacks[++m_handleGenerator] = a_callback;
        return m_handleGenerator;
    }
    
    uint32_t bind(Intern_Callback&& a_callback)
    {
        m_callbacks[++m_handleGenerator] = a_callback;
        return m_handleGenerator;
    }

    void unbind(const uint32_t a_handle)
    {
        if (auto iter = m_callbacks.find(a_handle); iter != m_callbacks.end())
            m_callbacks.erase(iter);
    }

    void broadcast(Args... a_arguments)const
    {
        for(const auto& [id, callback] : m_callbacks)
            callback(a_arguments...);
    }
};