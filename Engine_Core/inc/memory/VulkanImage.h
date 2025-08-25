#pragma once
/***********************************************
* @headerfile VulkanBuffer.h
* @date 10 / 08 / 2025
* @author Roomain
************************************************/
#include "vulkan/vulkan.hpp"
#include "vk_mem_alloc.h"


struct VulkanImage
{
	VkImage m_image = VK_NULL_HANDLE;                    /*!< vulkan image handle*/
	VmaAllocation m_Alloc = VK_NULL_HANDLE;
	VmaAllocationInfo m_AllocInfo;
};