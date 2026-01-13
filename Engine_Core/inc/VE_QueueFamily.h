#pragma once
/***********************************************
* @headerfile VE_QueueFamily.h
* @date 15 / 07 / 2025
* @author Roomain
************************************************/
#include <functional>
#include "vulkan/vulkan.hpp"
#include "utils/VulkanContext.h"
#include "iterators.h"
#include "core_globals.h"

#pragma warning(push)
#pragma warning( disable : 4251 )

/*@brief represents a command buffer*/
class VE_CORE_LIB VE_CommandBuffer : public VulkanObject<VE_DeviceContext>
{
	friend class VE_QueueFamily;
private:
	VkCommandBuffer m_cmdBuffer;	/*!< vulkan command buffer*/
	VkCommandPool m_cmdPool;		/*!< vulkan command pool used to release buffer*/

	explicit VE_CommandBuffer(VkCommandBuffer a_cmdBuffer, VkCommandPool a_cmdPool, const VE_DeviceContext& a_ctxt);
public:
	VE_CommandBuffer() = delete;
	VE_CommandBuffer(VE_CommandBuffer&& a_other) noexcept : VulkanObject<VE_DeviceContext>(a_other.m_vkCtxt),
		m_cmdBuffer{ a_other.m_cmdBuffer },
		m_cmdPool{ a_other.m_cmdPool }
	{
		// no code
	}
	NOT_COPIABLE(VE_CommandBuffer)
	~VE_CommandBuffer() override;
	void operator = (VE_CommandBuffer&& a_other) noexcept
	{
		m_cmdBuffer = a_other.m_cmdBuffer;
		m_cmdPool = a_other.m_cmdPool;
		m_vkCtxt = a_other.m_vkCtxt;
	}
	[[nodiscard]] constexpr bool isValid()const { return m_cmdBuffer != VK_NULL_HANDLE; }
	inline explicit operator VkCommandBuffer() { return m_cmdBuffer; }
	[[nodiscard]] inline VkCommandBuffer get() { return m_cmdBuffer; }
};

/*@brief represents a group of command buffers*/
class VE_CORE_LIB VE_CommandBufferPool : public VulkanObject<VE_DeviceContext>
{
	friend class VE_QueueFamily;
private:
	std::vector<VkCommandBuffer> m_cmdBuffers;	/*!< vulkan command buffers*/
	VkCommandPool m_cmdPool;					/*!< vulkan command pool used to release buffer*/

	explicit VE_CommandBufferPool(VkCommandPool a_cmdPool, const VkCommandBufferLevel a_level, const uint32_t a_bufferCount, const VE_DeviceContext& a_ctxt);
public:
	VE_CommandBufferPool() = delete;
	NOT_COPIABLE(VE_CommandBufferPool)
	~VE_CommandBufferPool() override;
	[[nodiscard]] constexpr uint32_t bufferCount()const { return static_cast<uint32_t>(m_cmdBuffers.size()); }
	[[nodiscard]] inline VkCommandBuffer& operator[] (const uint32_t a_index) { return m_cmdBuffers[a_index]; }
	DEFINE_ALL_ITER(std::vector<VkCommandBuffer>, m_cmdBuffers)

};

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
	explicit VE_QueueFamily(const uint32_t a_queueFamilyIndex, const VE_DeviceContext& a_ctxt);
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

	[[nodiscard]] VE_CommandBuffer createCommandBuffer(const VkCommandBufferLevel a_level);
	[[nodiscard]] VE_CommandBufferPool createCommandBuffer(const VkCommandBufferLevel a_level, const uint32_t a_count);
};

#pragma warning(pop)