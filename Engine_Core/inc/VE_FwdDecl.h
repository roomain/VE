#pragma once
/***********************************************
* @headerfile VE_FwdDecl.h
* @date 18 / 08 / 2025
* @author Roomain
************************************************/
#include <memory>

/*@brief forward declarations*/
class VE_Device;
class VE_QueueFamily;
struct VE_Buffer;

/*shared pointers*/
using VE_DevicePtr = std::shared_ptr<VE_Device>;
using VE_QueueFamilyPtr = std::shared_ptr<VE_QueueFamily>;

/*weak pointers*/
using VE_DeviceWptr = std::weak_ptr<VE_Device>;
using VE_QueueFamilyWptr = std::weak_ptr<VE_QueueFamily>;