#pragma once
/***********************************************
* @headerfile VE_Memory.h
* @date 24 / 08 / 2025
* @author Roomain
************************************************/
#include <vector>
#include "vulkan/vulkan.hpp"
#include "utils/VulkanContext.h"
#include "vk_mem_alloc.h"
#include "memory/VulkanBuffer.h"

struct VulkanImage;

struct VE_AllocationParameter
{
	size_t m_size;								/*!< memory size to allocate (in bytes)*/
	VkBufferUsageFlags m_usage;					/*!< buffer usage flags*/
	VmaAllocationCreateFlagBits m_allocFlag;	/*!< allocation flag bits*/
};

/*@brief create vulkan buffer in Gpu/Device Memory shared by queues*/
[[nodiscard]] VulkanBuffer allocateBuffer(const VE_DeviceContext& a_context, const VE_AllocationParameter& a_allocParameters, const std::vector<uint32_t>& a_sharedQueue);

/*@brief create vulkan buffer in Gpu/Device Memory NOT shared*/
[[nodiscard]] VulkanBuffer allocateBuffer(const VE_DeviceContext& a_context, const VE_AllocationParameter& a_allocParameters);

/*@brief create temporary buffer before sending to Gpu*/
[[nodiscard]] VulkanBuffer allocateStagingBuffer(const VE_DeviceContext& a_context, const size_t& a_bufferByteSize);

/*@brief release buffer*/
void releaseBuffer(const VE_DeviceContext& a_context, VulkanBuffer& a_buffer);

/*@brief write buffer data*/
template<typename Type>
void writeBuffer(const VE_DeviceContext& a_context, const VulkanBuffer& a_buffer, const Type* a_inputBuffer)
{
	VK_CHECK_EXCEPT(vmaFlushAllocation(a_context.m_memAllocator, a_buffer.m_Alloc, 0, sizeof(Type)));
	void* mapData = nullptr;
	VK_CHECK_EXCEPT(vmaMapMemory(a_context.m_memAllocator, a_buffer.m_Alloc, &mapData));
	std::memcpy(mapData, a_inputBuffer, sizeof(Type));
	vmaUnmapMemory(a_context.m_memAllocator, a_buffer.m_Alloc);
}


[[nodiscard]] VulkanImage allocateStagingImage(const VE_DeviceContext& a_context, const VkImageCreateInfo& a_imageCreateInfo);
[[nodiscard]] VulkanImage allocateImage(const VE_DeviceContext& a_context, const VkImageCreateInfo& a_imageCreateInfo);
void releaseImage(const VE_DeviceContext& a_context, VulkanImage& a_image);