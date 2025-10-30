#pragma once
/***********************************************
* @headerfile TGroupedTaskManager.h
* @date 26 / 10 / 2025
* @author Roomain
************************************************/
#include <memory>
#include <ranges>
#include <vector>
#include <algorithm>
#include <type_traits>
#include "TGroupedTaskInstance.h"
#include "TaskSynchro.h"

template<typename DataType>
using TaskPtr = std::shared_ptr<TGroupedTaskInstance<DataType>>;


/*@brief Manage a group of tasks working on same data type and
* wait for all task completion
*/
template<typename DataType, typename TaskType = TGroupedTaskInstance<DataType>> requires
std::is_base_of_v<TGroupedTaskInstance<DataType>, TaskType>
class TGroupedTaskManager
{
protected:
    using TaskPtr = std::shared_ptr<TaskType>;

    std::vector<TaskPtr> m_vTask;                /*!< taskList executed in parallel*/
    TaskSynchroPtr m_pSynchro;                   /*!< task synchro*/
    std::vector<unsigned int> m_vIndexToStop;    /*!< indexes of task to stop*/
    std::vector<unsigned int> m_vIndexStandby; /*!< indexes of task standby*/

    /*@brief create tasks but no process function attributed*/
    void createTasks(const int a_taskCount)
    {
        m_vTask.resize(a_taskCount);
        m_pSynchro = std::make_shared<TaskSynchro>(a_taskCount + 1);

        for (auto& task : m_vTask)
            task = std::make_shared<TaskType>(m_pSynchro);
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

    void unsetAllStandby()
    {
        for (const auto& task : m_vTask)
            task->setStandBy(false);
    }

    void setStandby()
    {
        std::ranges::sort(m_vIndexStandby);

        unsetAllStandby();

        m_vIndexStandby.erase(std::remove_if(m_vIndexStandby.begin(), m_vIndexStandby.end(),
            [&](int x) {
                return std::find(m_vIndexToStop.cbegin(), m_vIndexToStop.cend(), x) != m_vIndexToStop.cend();
            }),
            m_vIndexStandby.end()
        );

        for (auto index : m_vIndexStandby)
            m_vTask[index]->setStandBy(true);

        m_pSynchro->resetBarrier(m_vTask.size() - m_vIndexStandby.size() + 1);
    }

    void stopTasks()
    {
        std::ranges::sort(m_vIndexToStop);
        for(auto index : m_vIndexToStop)
            m_vTask[index]->stop();
    }

    void releaseTasks()
    {
        unsigned int offset = 0;
        // m_vIndexToStop and m_vIndexStandby are supposed sorted
        for (const auto index : m_vIndexToStop)
        {
            unsigned int taskIndex = index - offset;
            m_vTask[taskIndex]->join();
            m_vTask.erase(m_vTask.begin() + taskIndex);

            for (auto& sandbyIndex : m_vIndexStandby)
            {
                if (sandbyIndex > taskIndex)
                    sandbyIndex--;
            }
            offset++;
        }
        m_vIndexToStop.clear();
    }

public:

    ~TGroupedTaskManager()
    {
        m_vIndexToStop.clear();
        // release all stand by
        unsetAllStandby();

        for (const auto& task : m_vTask)
            task->stop();

        processTasks();

        for (const auto& task : m_vTask)
            task->join();
    }

    void appendTaskToStop(const int a_taskIndex)
    {
        if(m_vTask.size() > a_taskIndex &&
            std::ranges::find(m_vIndexToStop, a_taskIndex) == m_vIndexToStop.cend())
            m_vIndexToStop.emplace_back(a_taskIndex);
    }

    void appendTasksToStop(const std::vector<unsigned int>& a_taskIndexes)
    {
        for(const auto index : a_taskIndexes)
            appendTaskToStop(index);
    }

    void appendStandbyTask(const int a_taskIndex)
    {
        if (m_vTask.size() > a_taskIndex && 
            std::ranges::find(m_vIndexStandby, a_taskIndex) == m_vIndexStandby.cend())
            m_vIndexStandby.emplace_back(a_taskIndex);
    }

    void appendStandbyTasks(const std::vector<unsigned int>& a_taskIndexes)
    {
        for (const auto index : a_taskIndexes)
            appendStandbyTask(index);
    }

    void releaseStandBy(const unsigned int a_taskIndex)
    {
        if (auto iter = std::ranges::find(m_vIndexStandby, a_taskIndex); iter != m_vIndexStandby.cend())
        {
            m_vIndexStandby.erase(iter);
        }
    }

    void releaseStandBy(const std::vector<unsigned int>& a_taskIndexes)
    {
        for (const auto index : a_taskIndexes)
            releaseStandBy(index);
    }

    void releaseAllStandby()
    {
        m_vIndexStandby.clear();
    }

    constexpr size_t taskCount()const { return m_vTask.size(); }
    constexpr size_t taskToStopCount()const { return m_vIndexToStop.size(); }
    constexpr size_t taskStandbyCount()const { return m_vIndexStandby.size(); }

    using Standby_constIterator = std::vector<unsigned int>::const_iterator;
    Standby_constIterator cBeginStandby()const { return m_vIndexStandby.cbegin(); }
    Standby_constIterator cEndStandby()const { return m_vIndexStandby.cend(); }

    void process()
    {
        // set standby
        setStandby();

        stopTasks();

        processTasks();

        // set remove
        releaseTasks();

        finishProcess();
    }
};