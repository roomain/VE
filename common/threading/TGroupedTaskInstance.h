#pragma once
/***********************************************
* @headerfile TGroupedTaskInstance.h
* @date 26 / 10 / 2025
* @author Roomain
************************************************/
#include <thread>
#include <barrier>
#include <functional>
#include "TaskSynchro.h"

template<typename Type>
using TTaskProcess = std::function<void(Type&)>;

/*@brief Base class for task. GroupedTask is designed to 
* accomplish same action in parallel on different data. 
* Group of task is synchronised to be sure they're all
* accomplished.
*/
template<typename Data>
class TGroupedTaskInstance
{
protected:
    Data m_workedData;                      /*!< working data in thread*/

private:
    std::atomic_bool m_isRunning = false;   /*!< flag indicates thread is working*/
    std::atomic_bool m_standby = false;     /*!< standby mode*/
    std::thread m_thread;                   /*!< working thread*/
    TTaskProcess<Data> m_process;           /*!< process callback*/
    TaskSynchroPtr m_pSynchro;              /*!< task synchro*/

    void run()
    {
        if(m_pSynchro && m_process)
        {
            std::mutex protectCond;
            while(m_isRunning)
            {
                m_pSynchro->waitForStart(protectCond, m_standby);

                if(m_isRunning)
                {
                    m_process(m_workedData);
                }
                
                m_pSynchro->waitAllFinish();
            }
        }
    }


public:
    TGroupedTaskInstance() = delete;
    explicit TGroupedTaskInstance(TaskSynchroPtr a_pSynchro) : m_pSynchro{ a_pSynchro } {}
    
    void setData(Data&& a_workingData)
    {
        m_workedData = a_workingData;
    }

    void setData(const Data& a_workingData)
    {
        m_workedData = a_workingData;
    }

    inline void start() 
    { 
        m_isRunning = true;
        m_thread = std::thread(std::bind_front(&TGroupedTaskInstance<Data>::run, this));
    }
    
    inline void stop() 
    { 
        m_isRunning = false; 
        m_standby = false;
    }

    void setStandBy(const bool a_standby)
    {
        m_standby = a_standby;
    }

    [[nodiscard]] bool isStandBy()const
    {
        return m_standby;
    }

    inline void setSync(TaskSynchroPtr a_condCb)
    {
        m_pSynchro = a_condCb;
    }

    void setCallback(const TTaskProcess<Data>& a_callback) { m_process = a_callback; }
    void join() { m_thread.join(); }
};