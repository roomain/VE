#pragma once

#include <string>

#define BEGIN_ENUM_TO_STRING(Type) \
static [[nodiscard]] constexpr std::string to_string(const Type a_value) \
{ \
    std::string strValue;/* = #Type;*/ \
    switch(a_value) \
    {

#define ENUM_TO_STRING(value) \
    case value: \
        strValue += /*std::string("::") +*/ #value; \
        break;


#define END_ENUM_TO_STRING \
    } \
    return strValue; \
}

//---------------------------------------------------------------------------------------------------------------------

template<typename T> class Flag {};

#define BEGIN_FLAG_TO_STRING(Base, Type) \
template<> \
class Flag<Base>\
{\
public: \
    static [[nodiscard]] constexpr std::string to_string(const Type a_value) \
    { \
        unsigned int iValue = static_cast<unsigned int>(a_value);\
        std::string strValue;
   
#define FLAG_TO_STRING(value) \
        if((iValue & static_cast<unsigned int>(value)) ==  static_cast<unsigned int>(value)) \
        {\
            if(strValue.size() > 0) \
                strValue += std::string("|"); \
            strValue += ::to_string(value); \
        }
    
#define END_FLAG_TO_STRING \
        return strValue; \
    }\
};