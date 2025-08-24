#pragma once
/***********************************************
* @headerfile VulkanContext.h
* @date 15 / 07 / 2025
* @author Roomain
************************************************/
#include <type_traits>
#include <vulkan/vulkan.hpp>
#include "vk_mem_alloc.h"


/*brief vulkan context used for object using vulkan functions*/
struct VE_InstanceContext
{
	VkInstance m_instance = VK_NULL_HANDLE;	/*!< vulkan instance*/

	VE_InstanceContext() = default;
	explicit VE_InstanceContext(const VkInstance& a_instance) : m_instance{ a_instance } {}
	explicit VE_InstanceContext(const VE_InstanceContext& a_other) : m_instance{ a_other.m_instance } {}
	inline VE_InstanceContext& operator = (const VE_InstanceContext& a_other)
	{
		m_instance = a_other.m_instance;
		return *this;
	}
};

struct VE_DeviceContext : VE_InstanceContext
{
	int m_deviceIndex = -1;
	VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;
	VkDevice m_logicalDevice = VK_NULL_HANDLE; 
	VmaAllocator m_memAllocator = VK_NULL_HANDLE;

	VE_DeviceContext() = default;
	VE_DeviceContext(const VE_DeviceContext& a_other) = default;
	explicit VE_DeviceContext(const VE_InstanceContext& a_other) : VE_InstanceContext{ a_other }
	{
	}

	inline VE_DeviceContext& operator = (const VE_InstanceContext& a_other)
	{
		VE_InstanceContext::operator = (a_other);
		return *this;
	}

	inline VE_DeviceContext& operator = (const VE_DeviceContext& a_other)
	{
		VE_InstanceContext::operator = (a_other);
		m_physicalDevice = a_other.m_physicalDevice;
		m_logicalDevice = a_other.m_logicalDevice;
		m_memAllocator = a_other.m_memAllocator;
		return *this;
	}
};


struct VE_SwapChainContext : VE_DeviceContext
{
	VkSurfaceKHR m_surface = VK_NULL_HANDLE;
	VkSwapchainKHR m_swapChain = VK_NULL_HANDLE;

	VE_SwapChainContext() = default;
	VE_SwapChainContext(const VE_SwapChainContext& a_other) = default;
	explicit VE_SwapChainContext(const VE_DeviceContext& a_other) : VE_DeviceContext{ a_other }
	{
	}
	explicit VE_SwapChainContext(const VE_InstanceContext& a_other) : VE_DeviceContext{ a_other }
	{
	}
	inline VE_SwapChainContext& operator = (const VE_InstanceContext& a_other)
	{
		VE_InstanceContext::operator = (a_other);
		return *this;
	}

	inline VE_SwapChainContext& operator = (const VE_DeviceContext& a_other)
	{
		VE_DeviceContext::operator = (a_other);
		return *this;
	}

	inline VE_SwapChainContext& operator = (const VE_SwapChainContext& a_other)
	{
		VE_DeviceContext::operator = (a_other);
		m_surface = a_other.m_surface;
		m_swapChain = a_other.m_swapChain;
		return *this;
	}
};

template<typename Type>
using IsVulkanContext = std::is_base_of<VE_InstanceContext, Type>;

template<typename Type>
using IsVulkanDeviceContext = std::is_base_of<VE_DeviceContext, Type>;

template<typename Type>
using IsVulkanSwapChainContext = std::is_base_of<VE_SwapChainContext, Type>;

template<typename VkContext> requires IsVulkanContext<VkContext>::value
class VulkanObject 
{
protected:
	VkContext m_vkCtxt;

public:
	VulkanObject() = delete;
	virtual ~VulkanObject() = default;
	template<typename OtherContext>
	explicit VulkanObject(const OtherContext& a_ctxt) : m_vkCtxt{ a_ctxt } {}
	const VkContext& context()const noexcept { return m_vkCtxt; }
};