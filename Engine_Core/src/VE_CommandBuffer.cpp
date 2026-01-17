#include "pch.h"
#include "VE_CommandBuffer.h"

VE_CommandBuffer::VE_CommandBuffer(VkCommandBuffer a_cmdBuffer, VkCommandPool a_cmdPool, const VE_DeviceContext& a_ctxt) :
	VulkanObject<VE_DeviceContext>(a_ctxt), m_cmdBuffer{ a_cmdBuffer }, m_cmdPool{ a_cmdPool }
{
	//
}

VE_CommandBuffer::~VE_CommandBuffer()
{
	vkFreeCommandBuffers(m_vkCtxt.m_logicalDevice, m_cmdPool, 1, &m_cmdBuffer);
}

VE_CommandBufferPool::VE_CommandBufferPool(VkCommandPool a_cmdPool, const VkCommandBufferLevel a_level, const uint32_t a_bufferCount, const VE_DeviceContext& a_ctxt) :
	VulkanObject<VE_DeviceContext>(a_ctxt), m_cmdPool{ a_cmdPool }
{
	m_cmdBuffers.reserve(a_bufferCount);
	VkCommandBufferAllocateInfo allocInfo = Vulkan::Initializers::commandBufferCreateInfo(a_cmdPool, a_level, a_bufferCount);
	vkAllocateCommandBuffers(m_vkCtxt.m_logicalDevice, &allocInfo, m_cmdBuffers.data());
}

VE_CommandBufferPool::~VE_CommandBufferPool()
{
	vkFreeCommandBuffers(m_vkCtxt.m_logicalDevice, m_cmdPool, bufferCount(), m_cmdBuffers.data());
}
