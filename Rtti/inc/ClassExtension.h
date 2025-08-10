#pragma once
/***********************************************
* @headerfile ClassExtension.h
* @date 04 / 04 / 2025
* @author Romain Crespel
************************************************/
#include "Rtti_macros.h"

/*@brief base class to bring new features in an existing class*/
class ClassExtension
{
	DECLARE_RTTI(1, ClassExtension)
public:
	ClassExtension() = default;
	virtual ~ClassExtension() = default;
};
