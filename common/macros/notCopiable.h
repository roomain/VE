#pragma once
/***********************************************
* @headerfile logData.h
* @date 16 / 08 / 2024
* @author Roomain
************************************************/

#define NOT_COPIABLE(classname) \
classname(const classname&) = delete; \
classname& operator = (const classname&) = delete; 


#define NOT_MOVABLE(classname)\
classname(classname&&) noexcept = delete;\
classname& operator = (classname&&)noexcept = delete;