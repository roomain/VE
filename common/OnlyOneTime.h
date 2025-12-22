/***********************************************
* @headerfile OnlyOneTime.h
* @date 30 / 10 / 2025
* @author Roomain
************************************************/
#include <atomic>
#include "notCopiable.h"


/*@brief do action only one time*/
template<typename Action>
class OnlyOneTime
{
    Action m_action;                    /*!< action to do*/
    std::atomic_bool m_isDone = false;  /*!< indicate if action is already done*/

public:
    OnlyOneTime() = delete;
    NOT_COPIABLE(OnlyOneTime)
    explicit OnlyOneTime(const Action& action) : m_action{ action } {}
    explicit OnlyOneTime(Action&& action)noexcept : m_action{ action } {}
    void operator ()()
    {
        if (!m_isDone)
        {
            m_action();
            m_isDone = true;
        }
    }
    [[nodiscard]] bool isExecuted()const { return m_isDone; }
};
