#pragma once

/***********************************************
* @headerfile TEndlessTask.h
* @date 07 / 04 / 2025
* @author Roomain
************************************************/
#include <mutex>
#include <utility>
#include <functional>
#include <type_traits>
#include "common/notCopiable.h"

template<typename InOutData>
using InOutTask = std::function<void(InOutData&)>;
/* 
* @brief template class for endless task
* sharing data with another asynchronous thread
*/
template<typename SharedData, typename Task> requires std::is_same_v<Task, InOutTask<SharedData>>
class TEndlessTask
{
private:
    mutable std::mutex m_accessProtect;     /*!< access protection*/
    SharedData m_input;                     /*!< input data to process*/
    SharedData m_output;                    /*!< output data processed*/

    /*@brief swap data to start/finish process */
    void swap()
    {
        std::scoped_lock lock(m_accessProtect);
        std::swap(m_input, m_output);
    }

protected:
    inline void process()
    {
        // get input
        swap();
        // process Task
        Task(m_output);
        // set output
        swap();
    }

public:
    TEndlessTask(const TEndlessTask<SharedData, Task>&) = delete;
    TEndlessTask<SharedData, Task>& operator = (const TEndlessTask<SharedData, Task>&) = delete;

    void set(const SharedData& a_input)
    {
        std::scoped_lock lock(m_accessProtect);
        m_input = a_input
    }

    void operator = (const SharedData& a_input)
    {
        set(a_input);
    }

    void set(SharedData&& a_input)noexcept
    {
        std::scoped_lock lock(m_accessProtect);
        m_input = a_input
    }

    void operator = (SharedData&& a_input)noexcept
    {
        set(a_input);
    }

    SharedData get()const
    {
        std::scoped_lock lock(m_accessProtect);
        return m_input;
    }
};