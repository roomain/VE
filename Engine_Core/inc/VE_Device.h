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
#include "memory/VulkanBuffer.h"
#include "core_globals.h"

struct VE_Buffer; // forward declaration

#pragma warning(push)
#pragma warning(disable:4251) // disable warning for dll export/import
class VE_CORE_LIB VE_Device : public VulkanObject<VE_DeviceContext>
{
	friend class VE_Application;

protected:	
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

};
#pragma warning(pop)