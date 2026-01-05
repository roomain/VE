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

    std::vector<TaskPtr> m_vTask;               /*!< taskList executed in parallel*/
    TaskSynchroPtr m_pSynchro;                  /*!< task synchro*/
    std::vector<unsigned int> m_vIndexToStop;   /*!< indexes of task to stop*/
    std::vector<unsigned int> m_vIndexStandby;  /*!< indexes of task standby*/

    /*@brief create tasks but no process function attributed*/
    template<typename TaskSubType, typename ...Args> requires std::is_base_of_v<TaskType, TaskSubType>
    void createTasks(const unsigned int a_taskCount, Args... a_args)
    {
        size_t numTasks = 0;
        if (m_pSynchro)
        {
            numTasks = m_vTask.size();
            m_vTask.resize(a_taskCount + numTasks);
            m_pSynchro->resetBarrier(numTasks + a_taskCount +1);
        }
        else
        {
            m_vTask.resize(a_taskCount);
            m_pSynchro = std::make_shared<TaskSynchro>(a_taskCount + 1);
        }
    
        std::for_each(m_vTask.begin() + numTasks, m_vTask.end(), [&a_args..., this](auto& task) { task = std::make_shared<TaskSubType>(m_pSynchro, a_args...); });
    }

    template<typename TaskSubType, typename ...Args> requires std::is_base_of_v<TaskType, TaskSubType>
    void insertTask(Args... a_args)
    {
        if (m_pSynchro)
        {
            size_t numTasks = m_vTask.size() + 1;
            m_pSynchro->resetBarrier(numTasks + 1);
            m_vTask.insert(m_vTask.begin(), std::make_shared<TaskSubType>(m_pSynchro, a_args...));

        }
        else
        {
            m_pSynchro = std::make_shared<TaskSynchro>(1 + 1);
            m_vTask.emplace_back(std::make_shared<TaskSubType>(m_pSynchro, a_args...));
        }
    }


    template<typename TaskSubType, typename Arg> requires std::is_base_of_v<TaskType, TaskSubType>
    void createTasks(const std::vector<Arg>& a_args)
    {
        size_t numTasks = 0;
        if (m_pSynchro)
        {
            numTasks = m_vTask.size();
            m_pSynchro->resetBarrier(numTasks + a_args.size() + 1);
        }
        else
        {
            m_pSynchro = std::make_shared<TaskSynchro>(a_args.size() + 1);
        }

        for(const auto& argument : a_args)
            m_vTask.emplace_back(std::make_shared<TaskSubType>(m_pSynchro, argument));
    }

    /*@brief call when tasks will start*/
    virtual void startProcess() = 0;

    /*@brief call when all tasks are completed*/
    virtual void finishProcess() = 0;

    void processTasks()
    {
        // start all tasks
        m_pSynchro->start();

        // wait for all task completion
        m_pSynchro->waitAllFinish();
    }

    void unsetAllStandby()const
    {
        for (const auto& task : m_vTask)
            task->setStandBy(false);
    }

    void setStandby()
    {
        std::ranges::sort(m_vIndexStandby);

        unsetAllStandby();

#pragma warning(push)
#pragma warning( disable : 4858 )// because unsed local variable
        std::remove_if(m_vIndexStandby.begin(), m_vIndexStandby.end(),
            [&](const unsigned a_index) {
                return std::any_of(m_vIndexToStop.cbegin(), m_vIndexToStop.cend(), [a_index](const auto a_toStop)
                    {
                        return a_toStop == a_index;
                    });
            });
#pragma warning(pop)

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

    virtual ~TGroupedTaskManager()
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
        // set task to standby in standby mode
        setStandby();

        // set task to stop in stop mode
        stopTasks();

        // callback when process will start
        startProcess();

        // process active tasks
        processTasks();

        // remove tasks
        releaseTasks();

        // callback when all process are finished
        finishProcess();
    }
};