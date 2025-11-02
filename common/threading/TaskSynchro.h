#pragma once
/***********************************************
* @headerfile TaskSynchro.h
* @date 29 / 10 / 2025
* @author Roomain
************************************************/
#include <barrier>
#include <mutex>
#include <memory>

/*@brief synchronize grouped task*/
class TaskSynchro
{
public:
    // start synchronisation
    bool m_start = false;                       /*!< start flag*/
    std::condition_variable m_startConditionVar;/*!< start condition variable*/

    // finish synchronisation
    std::unique_ptr<std::barrier<>> m_finishBarrier;

public:
    TaskSynchro() = delete;
    explicit TaskSynchro(const ptrdiff_t& a_barrierCount)
    {
        m_finishBarrier = std::make_unique<std::barrier<>>(a_barrierCount);
    }

    //---------------------------------------------------------------
    // manager part
    //---------------------------------------------------------------
    inline void resetBarrier(const ptrdiff_t& a_barrierCount)
    {
        m_finishBarrier = std::make_unique<std::barrier<>>(a_barrierCount);
    }

    inline void start()
    {
        m_start = true;
        m_startConditionVar.notify_all();
        m_finishBarrier->arrive_and_wait();
        m_start = false;
    }

    //---------------------------------------------------------------
    // task part
    //---------------------------------------------------------------
    void waitForStart(std::mutex& a_localProtect, const bool& a_externalCond = true)
    {
        std::unique_lock lock(a_localProtect);
        m_startConditionVar.wait(lock, [&] {return a_externalCond && m_start; });
        m_finishBarrier->arrive_and_wait();
    }

    //---------------------------------------------------------------
    // both part
    //---------------------------------------------------------------
    void waitAllFinish()
    {
        m_finishBarrier->arrive_and_wait();
    }
};

using TaskSynchroPtr = std::shared_ptr<TaskSynchro>;