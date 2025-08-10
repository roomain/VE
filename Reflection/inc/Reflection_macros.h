#pragma once
#include "ReflectionManager.h"

#pragma message("-> Don't forget REFLECT macro and include generated header IN FILE")
#pragma message("-> Don't forget include deserialize_functions.h and deserialize surcharge for members")

// macros used by reflection script
#define REFLECT_CLASS(name)
#define REFLECT_MEMBER
#define REFLECT_FLAG(baseEnum)
//------------------------------------------------------

/*buffer that contains base instance data*/
template<typename Type>
struct RefectionBuffer
{
    bool isInitialized = false;
    Type baseInstanceData;
};

#define DESERIALIZE_DECLARE(classname)                          \
friend class ReflectionManager;                                 \
private:                                                        \
static RefectionBuffer<classname> m_reflectionBuffer;           \
public:                                                         \
static void deserialize(classname* const a_this);               \
static inline std::string getClassname() { return #classname; } \
public:                                                         \
classname();                                                    \
                                                                                        

#define DESERIALIZE_IMPL(classname)                                       \
RefectionBuffer<classname> classname::m_reflectionBuffer;                 \
                                                                          \
void classname::deserialize(classname* const a_this)                      \
{                                                                         \
    auto& ReflectMgr = ReflectionManager::instance();                     \
    if(auto pClass = ReflectMgr.getReflection(classname::getClassname())) \
    {                                                                     \
        _DESERIALIZE_##classname(pClass);                                 \
    }                                                                     \
}                                                                         \
                                                                          \
classname::classname()                                                    \
{                                                                         \
    if(!m_reflectionBuffer.isInitialized)                                 \
    {                                                                     \
        classname::deserialize(m_reflectionBuffer.baseInstanceData);      \
        m_reflectionBuffer.isInitialized = true;                          \
    }                                                                     \
    *this = m_reflectionBuffer.baseInstanceData;                          \
}                                                                         \



#define DESERIALIZE_INLINE_IMPL(classname)                                \
inline void classname::deserialize(classname* const a_this)               \
{                                                                         \
    auto& ReflectMgr = ReflectionManager::instance();                     \
    if(auto pClass = ReflectMgr.getReflection(classname::getClassname())) \
    {                                                                     \
        _DESERIALIZE_##classname(pClass);                                 \
    }                                                                     \
}                                                                         \
                                                                          \
inline classname::classname()                                             \
{                                                                         \
    classname::deserialize(this);                                         \
}   