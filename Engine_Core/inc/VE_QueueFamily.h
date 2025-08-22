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

#pragma warning(push)
#pragma warning( disable : 4251 )

class VE_CORE_LIB VE_QueueFamily : public VulkanObject<VE_DeviceContext>
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

	using CmbBufferConst_Iter = std::vector<VkCommandBuffer>::const_iterator;
	[[nodiscard]] inline CmbBufferConst_Iter commandBufferBegin() const noexcept {	return m_commandBuffers.cbegin(); }
	[[nodiscard]] inline CmbBufferConst_Iter commandBufferEnd() const noexcept { return m_commandBuffers.cend(); }
	void releaseCommandBuffers(CmbBufferConst_Iter a_begin, CmbBufferConst_Iter a_end);

	[[nodiscard]] VkCommandBuffer createCommandBuffer(const VkCommandBufferLevel a_level);
	[[nodiscard]] void createCommandBuffer(const VkCommandBufferLevel a_level, const uint32_t a_count);
	[[nodiscard]] VkCommandBuffer getCommandBuffer(const uint32_t a_index);
	[[nodiscard]] uint32_t numCommandBuffer()const;
};

#pragma warning(pop)