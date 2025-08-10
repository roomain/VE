#pragma once
/***********************************************
* @headerfile core_globals.h
* @date 24 / 10 / 2024
* @author Roomain
************************************************/

#ifdef VE_CORE_EXPORT
#define VE_CORE_LIB __declspec(dllexport)
#else
#define VE_CORE_LIB __declspec(dllimport)
#endif
