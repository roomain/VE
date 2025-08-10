#pragma once
/***********************************************
* @headerfile Reflectionglobals.h
* @date 24 / 10 / 2024
* @author Roomain
************************************************/

#ifdef REFLECTION_EXPORT
#define REFLECTION_LIB __declspec(dllexport)
#else
#define REFLECTION_LIB __declspec(dllimport)
#endif
