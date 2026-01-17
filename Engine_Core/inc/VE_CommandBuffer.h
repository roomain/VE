#pragma once
/***********************************************
* @headerfile VE_CommandBuffer.h
* @date 17 / 01 / 2026
* @author Roomain
************************************************/
#include <memory>
#include "vulkan/vulkan.hpp"
#include "utils/VulkanContext.h"
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
	inline explicit operator VkCommandBuffer&() { return m_cmdBuffer; }
	[[nodiscard]] inline VkCommandBuffer& get() { return m_cmdBuffer; }
};

using VE_CommandBufferPtr = std::shared_ptr<VE_CommandBuffer>;

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
using VE_CommandBufferPoolPtr = std::shared_ptr<VE_CommandBufferPool>;

#pragma warning(pop)