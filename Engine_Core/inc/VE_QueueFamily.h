#pragma once
/***********************************************
* @headerfile VE_QueueFamily.h
* @date 15 / 07 / 2025
* @author Roomain
************************************************/
#include <functional>
#include "vulkan/vulkan.hpp"
#include "utils/VulkanContext.h"
#include "core_globals.h"

class VE_QueueFamily;

class VE_CORE_LIB VE_CommandBuffer
{
	friend class VE_QueueFamily;
private:
	VkCommandBuffer m_commandBuffer = VK_NULL_HANDLE;				/*!< command buffer*/
	VkCommandBufferLevel m_level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;	/*!< command buffer level*/
    std::reference_wrapper<const VE_QueueFamily> m_queueFamily;		/*!< queue family associated to command buffer*/

	explicit VE_CommandBuffer(const VkCommandBuffer a_commandBuffer, const VkCommandBufferLevel a_level, const VE_QueueFamily& a_queueFamily) noexcept;

public:
	VE_CommandBuffer() = delete;
};


class VE_CORE_LIB VE_QueueFamily : private VulkanObject<VE_DeviceContext>
{
private:
	uint32_t m_familyIndex = 0;						/*!< queue family index*/
	uint32_t m_numQueueAvailable = 0;				/*!< available queues*/
	VkQueueFlags m_familyFlag = 0;					/*!< queue family flag*/
	VkCommandPool m_commandPool = VK_NULL_HANDLE;	/*!< command pool associated to queue family*/
	std::vector<VkQueue> m_queues;					/*!< used queues*/
	std::vector<VkCommandBuffer> m_commandBuffers;	/*!< instanciated command buffer*/
	std::vector<VkFence> m_waitFences;				/*!< instanciated fence*/

public:
	explicit VE_QueueFamily(const uint32_t a_queueFamilyIndex, const VE_DeviceContext& a_ctxt);
	virtual ~VE_QueueFamily() override;
	[[nodiscard]] VkCommandPool commandPool();
	[[nodiscard]] VkQueueFlags familyFlag()const noexcept;
	[[nodiscard]] uint32_t availableQueues()const noexcept;
	[[nodiscard]] VkQueue createCommandQueue();
	[[nodiscard]] VkQueue getQueue(const uint32_t a_index);
	[[nodiscard]] uint32_t numCommandQueues()const;

	[[nodiscard]] VkFence createFence();
	[[nodiscard]] VkFence getFence(const uint32_t a_index);
	[[nodiscard]] uint32_t numFences()const;

	[[nodiscard]] VkCommandBuffer createCommandBuffer(const VkCommandBufferLevel a_level);
	[[nodiscard]] void createCommandBuffer(const VkCommandBufferLevel a_level, const uint32_t a_count);
	[[nodiscard]] VkCommandBuffer getCommandBuffer(const uint32_t a_index);
	[[nodiscard]] uint32_t numCommandBuffer()const;
};
