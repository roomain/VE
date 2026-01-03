#pragma once
/***********************************************
* @headerfile VE_FwdDecl.h
* @date 18 / 08 / 2025
* @author Roomain
************************************************/
#include <memory>

/*@brief forward declarations*/
class VE_Device;
class VE_GraphicalDevice;
class VE_QueueFamily;
class VE_Shader;
class VE_Uniform;
class VE_Pipeline;
class VE_SwapChain;
class VE_CommandBuffer;

/*shared pointers*/
using VE_DevicePtr = std::shared_ptr<VE_Device>;
using VE_GraphicalDevicePtr = std::shared_ptr<VE_GraphicalDevice>;
using VE_QueueFamilyPtr = std::shared_ptr<VE_QueueFamily>;
using VE_ShaderPtr = std::shared_ptr<VE_Shader>;
using VE_UniformPtr = std::shared_ptr<VE_Uniform>;
using VE_PipelinePtr = std::shared_ptr<VE_Pipeline>;
using VE_SwapChainPtr = std::shared_ptr<VE_SwapChain>;

/*weak pointers*/
using VE_DeviceWptr = std::weak_ptr<VE_Device>;
using VE_GraphicalDeviceWPtr = std::weak_ptr<VE_GraphicalDevice>;
using VE_QueueFamilyWptr = std::weak_ptr<VE_QueueFamily>;
using VE_ShaderWptr = std::weak_ptr<VE_Shader>;
using VE_UniformWptr = std::weak_ptr<VE_Uniform>;
using VE_PipelineWptr = std::weak_ptr<VE_Pipeline>;
using VE_SwapChainWPtr = std::weak_ptr<VE_SwapChain>;