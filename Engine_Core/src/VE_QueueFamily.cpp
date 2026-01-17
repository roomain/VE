#include "pch.h"
#include "VE_QueueFamily.h"
#include "VE_Application.h"
#include "VE_CommandBuffer.h"
#include "utils/VulkanCmdInitializers.h"


//---------------------------------------------------------------------------------------------------------

VE_QueueFamily::VE_QueueFamily(const uint32_t a_queueFamilyIndex, const VE_DeviceContext& a_ctxt) :
	VulkanObject<VE_DeviceContext>(a_ctxt), m_familyIndex{ a_queueFamilyIndex }
{
	auto queue = (VE_Application::capabilities().deviceBegin() + a_ctxt.m_deviceIndex)->queueBegin() + a_queueFamilyIndex;
	m_numQueueAvailable = queue->queueCount;
	m_familyFlag = queue->queueFlags;
}

VE_QueueFamily::~VE_QueueFamily()
{
	for (auto& fence : m_waitFences)
		vkDestroyFence(m_vkCtxt.m_logicalDevice, fence, nullptr);

	vkDestroyCommandPool(m_vkCtxt.m_logicalDevice, m_commandPool, nullptr);
}

VkCommandPool VE_QueueFamily::commandPool()
{
	if (m_commandPool == VK_NULL_HANDLE)
	{
		VkCommandPoolCreateInfo createInfo = Vulkan::Initializers::commandPoolCreateInfo(VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT, m_familyIndex);
		VK_CHECK_EXCEPT(vkCreateCommandPool(m_vkCtxt.m_logicalDevice, &createInfo, nullptr, &m_commandPool));
	}
	return m_commandPool;
}

VkQueueFlags VE_QueueFamily::familyFlag()const noexcept
{
	return m_familyFlag;
}

uint32_t VE_QueueFamily::availableQueues()const noexcept
{
	return m_numQueueAvailable;
}

VkQueue VE_QueueFamily::createCommandQueue()
{
	if (m_numQueueAvailable > 0)
	{
		VkQueue queue = VK_NULL_HANDLE;
		vkGetDeviceQueue(m_vkCtxt.m_logicalDevice, m_familyIndex, static_cast<uint32_t>(m_queues.size()), &queue);
		if (queue != VK_NULL_HANDLE)
		{
			m_queues.emplace_back(queue);
			m_numQueueAvailable--;
			return queue;
		}
	}
	Logger::error(std::format("Can't create queue for device {} : no queue available", m_vkCtxt.m_deviceIndex));
	return VK_NULL_HANDLE;
}

VkQueue VE_QueueFamily::getQueue(const uint32_t a_index)
{
	return m_queues[a_index];
}

uint32_t VE_QueueFamily::numCommandQueues()const
{
	return static_cast<uint32_t>(m_queues.size());
}


VkFence VE_QueueFamily::createFence()
{
	VkFence fence = VK_NULL_HANDLE;
	VkFenceCreateInfo fenceCreateInfo = Vulkan::Initializers::fenceCreateInfo();
	VK_CHECK_LOG(vkCreateFence(m_vkCtxt.m_logicalDevice, &fenceCreateInfo, nullptr, &fence));
	m_waitFences.emplace_back(fence);
	return fence;
}

VkFence VE_QueueFamily::getFence(const uint32_t a_index)
{
	return m_waitFences[a_index];
}

uint32_t VE_QueueFamily::numFences()const
{
	return static_cast<uint32_t>(m_waitFences.size());
}

VE_CommandBufferPtr VE_QueueFamily::createCommandBuffer(const VkCommandBufferLevel a_level)
{
	VkCommandBufferAllocateInfo allocInfo = Vulkan::Initializers::commandBufferCreateInfo(commandPool(), a_level, 1);
	VkCommandBuffer cmdBuffer;
	vkAllocateCommandBuffers(m_vkCtxt.m_logicalDevice, &allocInfo, &cmdBuffer);
	// because ctor is private
	return VE_CommandBufferPtr(new VE_CommandBuffer(cmdBuffer, commandPool(), m_vkCtxt));
}

VE_CommandBufferPoolPtr VE_QueueFamily::createCommandBufferPool(const VkCommandBufferLevel a_level, const uint32_t a_count)
{
	// because ctor is private
	return VE_CommandBufferPoolPtr(new VE_CommandBufferPool(commandPool(), a_level, a_count, m_vkCtxt));
}
