#pragma once
/***********************************************
* @headerfile VE_Device.h
* @date 15 / 07 / 2025
* @author Roomain
************************************************/
#include <memory>
#include <vector>
#include "vulkan/vulkan.hpp"
#include "utils/VulkanContext.h"
#include "VE_QueueFamily.h"
#include "vk_mem_alloc.h"
#include "memory/VulkanBuffer.h"
#include "core_globals.h"

struct VulkanBuffer;
struct VE_Buffer; // forward declaration

#pragma warning(push)
#pragma warning(disable:4251) // disable warning for dll export/import
class VE_CORE_LIB VE_Device : public VulkanObject<VE_DeviceContext>
{
	friend class VE_Application;

protected:
	VmaAllocator m_memAllocator = VK_NULL_HANDLE;
	std::vector<VE_QueueFamily>	m_queues; /*!< queues availables/used by device*/

	void createMemoryAllocator();
	explicit VE_Device(const VE_DeviceContext& a_context, const std::vector<int>& a_queueFamilies);

public:
	virtual ~VE_Device() override;

#pragma region command
	void createCommandBuffers(const VkQueueFlags a_flag,
		const VkCommandBufferLevel a_cmdLevel, const uint32_t a_numBuffers, std::vector<VkCommandBuffer>& a_buffers);
	void createCommandBuffer(const VkQueueFlags a_flag,
		const VkCommandBufferLevel a_cmdLevel, VkCommandBuffer& a_buffer);
	VkQueue createQueue(const VkQueueFlags a_flag);
#pragma endregion

#pragma region buffer
	/*@brief create vulkan buffer in Gpu/Device Memory shared by queues*/
	[[nodiscard]] VulkanBuffer allocateBuffer(const size_t& a_bufferByteSize, 
		const VkBufferUsageFlags a_bufFlags,
		const VmaAllocationCreateFlagBits a_allocFlag, 
		const std::vector<uint32_t>& a_sharedQueue)const;

	/*@brief create vulkan buffer in Gpu/Device Memory NOT shared*/
	[[nodiscard]] VulkanBuffer allocateBuffer(const size_t& a_bufferByteSize, 
		const VkBufferUsageFlags a_bufFlags,
		const VmaAllocationCreateFlagBits a_allocFlag)const;

	/*@brief create temporary buffer brfore sending to Gpu*/
	[[nodiscard]] VulkanBuffer allocateStagingBuffer(const size_t& a_bufferByteSize)const;

	/*@brief release buffer*/
	void releaseBuffer(VulkanBuffer& a_buffer)const;

	/*@brief write buffer data*/
	template<typename Type>
	void writeBuffer(const VulkanBuffer& a_buffer, const Type* a_inputBuffer)const
	{
		VK_CHECK_EXCEPT(vmaFlushAllocation(m_memAllocator, a_buffer.m_Alloc, 0, sizeof(Type)));
		void* mapData = nullptr;
		VK_CHECK_EXCEPT(vmaMapMemory(m_memAllocator, a_buffer.m_Alloc, &mapData));
		std::memcpy(mapData, a_inputBuffer, sizeof(Type));
		vmaUnmapMemory(m_memAllocator, a_buffer.m_Alloc);
	}
#pragma endregion
};
#pragma warning(pop)