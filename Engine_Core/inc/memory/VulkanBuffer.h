#pragma once
/***********************************************
* @headerfile VulkanBuffer.h
* @date 10 / 08 / 2025
* @author Roomain
************************************************/
#include "vulkan/vulkan.hpp"

/*@brief structure with vulkan buffer handles*/
struct VulkanBuffer
{
	VkBuffer m_buffer = VK_NULL_HANDLE;					/*!< vulkan buffer handle*/
	VmaAllocation m_Alloc = VK_NULL_HANDLE;
	VmaAllocationInfo m_AllocInfo;
};
