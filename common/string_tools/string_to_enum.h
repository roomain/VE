#pragma once
#include <string>

#define BEGIN_STRING_TO_ENUM(Type) \
static [[nodiscard]] constexpr bool to_enum(const std::string &a_strValue, Type& a_typeValue) \
{ \

#define STRING_TO_ENUM(value) \
    if(a_strValue.compare(#value) == 0)\
    {\
        a_typeValue = value; \
        return true;\
    }\


#define END_STRING_TO_ENUM \
    return false; \
}



#define BEGIN_STRING_TO_FLAG(FlagBaseType, FlagType) \
static [[nodiscard]] constexpr bool to_flag(const std::string &a_strValue, FlagBaseType a_default, FlagType& a_typeValue) \
{ \
    a_typeValue = a_default;                                                \
    size_t pos = a_strValue.find('|');                                      \
    std::string sLeft, sRight;                                              \
    if (pos == std::string::npos)                                           \
    {                                                                       \
        sLeft = a_strValue;                                                 \
    }                                                                       \
    else                                                                    \
    {                                                                       \
        sLeft = a_strValue.substr(pos);                                     \
        sRight = a_strValue.substr(pos + 1, a_strValue.size() - pos - 1);   \
    }                                                                       \
                                                                            \
    while (!sLeft.empty())                                                  \
    {                                                                       \

#define STRING_TO_FLAG(value) \
        if(sLeft.compare(#value) == 0)         \
        {                                       \
            a_typeValue = a_typeValue | value;  \
        }else                                   \


#define END_STRING_TO_FLAG \
        {                                                                   \
            return false;                                                   \
        }                                                                   \
        if (!sRight.empty())                                                \
        {                                                                   \
            pos = sRight.find('|');                                         \
            if (pos == std::string::npos)                                   \
            {                                                               \
                sLeft = a_strValue;                                         \
                sRight = "";                                                \
            }                                                               \
            else                                                            \
            {                                                               \
                sLeft = sRight.substr(pos);                                 \
                sRight = sRight.substr(pos + 1, sRight.size() - pos - 1);   \
            }                                                               \
        }                                                                   \
        else                                                                \
        {                                                                   \
            sLeft = "";                                                     \
        }                                                                   \
    }                                                                       \
    return true;                                                            \
}                                                               

