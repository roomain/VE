#pragma once
/***********************************************
* @headerfile VE_QueueFamily.h
* @date 15 / 07 / 2025
* @author Roomain
************************************************/
#include <functional>
#include <memory>
#include "vulkan/vulkan.hpp"
#include "utils/VulkanContext.h"
#include "iterators.h"
#include "core_globals.h"

#pragma warning(push)
#pragma warning( disable : 4251 )

class VE_InstanceCapabilities;
class VE_CommandBuffer;
using VE_CommandBufferPtr = std::shared_ptr<VE_CommandBuffer>;

/*@brief represents a group of command buffers*/
class VE_CommandBufferPool;
using VE_CommandBufferPoolPtr = std::shared_ptr<VE_CommandBufferPool>;

class VE_CORE_LIB VE_QueueFamily : public VulkanObject<VE_DeviceContext>
{
private:
	uint32_t m_familyIndex = 0;						/*!< queue family index*/
	uint32_t m_numQueueAvailable = 0;				/*!< available queues*/
	VkQueueFlags m_familyFlag = 0;					/*!< queue family flag*/
	VkCommandPool m_commandPool = VK_NULL_HANDLE;	/*!< command pool associated to queue family*/
	std::vector<VkQueue> m_queues;					/*!< used queues*/
	std::vector<VkFence> m_waitFences;				/*!< instanciated fence*/

public:
	explicit VE_QueueFamily(const VE_InstanceCapabilities& a_capabilities, const uint32_t a_queueFamilyIndex, const VE_DeviceContext& a_ctxt);
	~VE_QueueFamily() override;
	[[nodiscard]] VkCommandPool commandPool();
	[[nodiscard]] VkQueueFlags familyFlag()const noexcept;
	[[nodiscard]] uint32_t availableQueues()const noexcept;
	[[nodiscard]] VkQueue createCommandQueue();
	[[nodiscard]] VkQueue getQueue(const uint32_t a_index);
	[[nodiscard]] uint32_t numCommandQueues()const;

	[[nodiscard]] VkFence createFence();
	[[nodiscard]] VkFence getFence(const uint32_t a_index);
	[[nodiscard]] uint32_t numFences()const;

	[[nodiscard]] VE_CommandBufferPtr createCommandBuffer(const VkCommandBufferLevel a_level);
	[[nodiscard]] VE_CommandBufferPoolPtr createCommandBufferPool(const VkCommandBufferLevel a_level, const uint32_t a_count);
};

#pragma warning(pop)