#pragma once
#include <string>
#include <unordered_map>
#include "ReflectionValue.h"
#include "ReflectionGlobals.h"

#pragma warning(push)
#pragma warning( disable : 4251 )
/*@brief reflection class*/
class REFLECTION_LIB ReflectionClass
{
    friend class ReflectionJsonParser;

private:
    std::unordered_map<std::string, ReflectionValue> m_classMembers;
    std::shared_ptr<ReflectionClass> m_nested;
    ReflectionClass() = default;

public:

    template<typename Type>
    void deserialize(const std::string& a_memberName, Type& m_member)const
    {
        if (m_nested)
        {
            m_nested->deserialize(a_memberName, m_member);
        }
        else
        {
            if (auto iter = m_classMembers.find(a_memberName); iter != m_classMembers.cend())
                iter->second.set(m_member);
        }
    }

    inline void overrideClass(const ReflectionClassPtr& a_overrided)
    {
        m_nested = a_overrided;
    }

    [[nodiscard]]inline  bool resetOverride()
    {
        bool bIsOverrided = m_nested.get() != nullptr;
        m_nested.reset();
        return bIsOverrided;
    }
};

#pragma warning(pop)