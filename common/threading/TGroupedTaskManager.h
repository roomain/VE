#pragma once
/***********************************************
* @headerfile TGroupedTaskManager.h
* @date 26 / 10 / 2025
* @author Roomain
************************************************/
#include <memory>
#include <type_traits>
#include "TGroupedTaskInstance.h"
#include "TaskSynchro.h"

template<typename DataType>
using TaskPtr = std::shared_ptr<TGroupedTaskInstance<DataType>>;


/*@brief Manage a group of tasks working on same data type and
* wait for all task completion
*/
template<typename DataType, typename TaskType> requires 
std::is_base_of_v<TGroupedTaskInstance<DataType>, TaskType>
class TGroupedTaskManager
{
protected:
    using TaskPtr = std::shared_ptr<TaskType>;

    std::vector<TaskPtr> m_vTask; /*!< taskList executed in parallel*/
    TaskSynchroPtr m_pSynchro;              /*!< task synchro*/

    /*@brief create tasks but no process function attributed*/
    void createTasks(const int a_taskCount)
    {
        m_vTask.resize(a_taskCount);
        m_pSynchro = std::make_shared<TaskSynchro>(a_taskCount + 1);
        for (auto& task : m_vTask)
        {
            // because ctor is private
            task = std::make_shared<TaskType>(m_pSynchro);
        }
    }

    /*@brief call when all task are completed*/
    virtual void finishProcess() = 0;

    void processTasks()
    {
        // start all tasks
        m_pSynchro->start();

        // wait for all task completion
        m_pSynchro->waitAllFinish();
    }

public:

    ~TGroupedTaskManager()
    {
        for (auto& task : m_vTask)
            task->stop();

        processTasks();

        for (auto& task : m_vTask)
            task->join();
    }

    void process()
    {
        processTasks();
        finishProcess();
    }
};