#include "pch.h"
#include <ranges>
#include <format>
#include "VE_Device.h"
#include "memory/VE_MemoryCallbacks.h"
#include "utils/VulkanBufferInitializers.h"
#include "utils/VulkanCmdInitializers.h"

void VE_Device::createMemoryAllocator()
{
	static const VkAllocationCallbacks cpuAllocationCallbacks = {
		nullptr, // pUserData
		&vulkanCustom_CpuAllocation, // pfnAllocation
		&vulkanCustom_CpuReallocation, // pfnReallocation
		&vulkanCustom_CpuFree // pfnFree
	};

	static VmaVulkanFunctions vulkanFunctions
	{
		.vkGetInstanceProcAddr = vkGetInstanceProcAddr,
		.vkGetDeviceProcAddr = vkGetDeviceProcAddr
	};

	VmaAllocatorCreateInfo vmaInfo
	{
		.flags = VMA_ALLOCATOR_CREATE_EXT_MEMORY_BUDGET_BIT | VMA_ALLOCATOR_CREATE_AMD_DEVICE_COHERENT_MEMORY_BIT |
		VMA_ALLOCATOR_CREATE_BUFFER_DEVICE_ADDRESS_BIT | VMA_ALLOCATOR_CREATE_EXT_MEMORY_PRIORITY_BIT /* |
		VMA_ALLOCATOR_CREATE_KHR_EXTERNAL_MEMORY_WIN32_BIT*/,
		.physicalDevice = m_vkCtxt.m_physicalDevice,
		.device = m_vkCtxt.m_logicalDevice,
		.preferredLargeHeapBlockSize = 0,
		.pAllocationCallbacks = &cpuAllocationCallbacks,
		.pDeviceMemoryCallbacks = nullptr,
		.pHeapSizeLimit = 0,
		.pVulkanFunctions = &vulkanFunctions,
		.instance = m_vkCtxt.m_instance,
		.vulkanApiVersion = VK_API_VERSION_1_3
	};

	VK_CHECK_EXCEPT(vmaCreateAllocator(&vmaInfo, &m_memAllocator))
}

VE_Device::VE_Device(const VE_DeviceContext& a_context, const std::vector<int>& a_queueFamilies) :
	VulkanObject<VE_DeviceContext>{ a_context }
{
	// create queue family
	for (const auto& familyIndex : a_queueFamilies)
		m_queues.emplace_back(familyIndex, m_vkCtxt);

	createMemoryAllocator();
}

VE_Device::~VE_Device()
{
	m_queues.clear();// destroy all queues
	vkDestroyDevice(m_vkCtxt.m_logicalDevice, nullptr);
}

void VE_Device::createCommandBuffers(const VkQueueFlags a_flag, const VkCommandBufferLevel a_cmdLevel, std::vector<VkCommandBuffer>& a_buffers, const uint32_t a_numBuffers)
{
	// find queue
	if (auto iter = std::ranges::find_if(m_queues, [a_flag](const auto& a_queueFam)
		{
			return (a_queueFam.familyFlag() & a_flag) == a_flag;
		}); iter != m_queues.cend())
	{
		VkCommandBufferAllocateInfo info = Vulkan::Initializers::commandBufferCreateInfo(
			iter->commandPool(),
			a_cmdLevel,
			a_numBuffers
		);
		a_buffers.resize(a_numBuffers);
		vkAllocateCommandBuffers(m_vkCtxt.m_logicalDevice, &info, a_buffers.data());
	}
	else
	{
		Logger::error(std::format("Can't create command buffer with queue flags {}", 
			Flag<VkQueueFlagBits>::to_string(a_flag)));
	}
}

void VE_Device::createCommandBuffer(const VkQueueFlags a_flag, const VkCommandBufferLevel a_cmdLevel, VkCommandBuffer& a_buffer)
{
	// find queue
	if (auto iter = std::ranges::find_if(m_queues, [a_flag](const auto& a_queueFam)
		{
			return (a_queueFam.familyFlag() & a_flag) == a_flag;
		}); iter != m_queues.cend())
	{
		VkCommandBufferAllocateInfo info = Vulkan::Initializers::commandBufferCreateInfo(
			iter->commandPool(),
			a_cmdLevel,
			1
		);
		vkAllocateCommandBuffers(m_vkCtxt.m_logicalDevice, &info, &a_buffer);
	}
	else
	{
		Logger::error(std::format("Can't create command buffer with queue flags {}",
			Flag<VkQueueFlagBits>::to_string(a_flag)));
	}
}

VkQueue VE_Device::createQueue(const VkQueueFlags a_flag)
{
	// find queue
	if (auto iter = std::ranges::find_if(m_queues, [a_flag](const auto& a_queueFam)
		{
			return (a_queueFam.familyFlag() & a_flag) == a_flag;
		}); iter != m_queues.cend())
	{
		return iter->createCommandQueue();
	}
	else
	{
		Logger::error(std::format("Can't create queue with queue flags {}",
			Flag<VkQueueFlagBits>::to_string(a_flag)));
	}
	return VK_NULL_HANDLE;
}


#pragma region buffer
void VE_Device::createStagingBuffer(const VkDeviceSize& a_size, VE_Buffer& a_buffer)const
{
	/*if (a_size == 0)
	{
		Logger::error("Staging buffer size is zero, can't create staging buffer");
		return;
	}

	VkBufferCreateInfo bufferInfo = Vulkan::Initializers::bufferCreateInfo(
		Vulkan::Initializers::BufferCreateInfoParameters{ 
			0, 
			a_size, 
			VK_BUFFER_USAGE_TRANSFER_SRC_BIT, 
			VK_SHARING_MODE_EXCLUSIVE,
			0,
			nullptr }
	);

	VmaAllocationCreateInfo stagingBufAllocCreateInfo{};
	stagingBufAllocCreateInfo.usage = VMA_MEMORY_USAGE_AUTO;
	stagingBufAllocCreateInfo.flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT | VMA_ALLOCATION_CREATE_MAPPED_BIT;

	VK_CHECK_EXCEPT(vmaCreateBuffer(m_memAllocator, 
		&bufferInfo, 
		&stagingBufAllocCreateInfo,
		&a_buffer.m_stagingBuffer, 
		&a_buffer.m_stagingBufAlloc, 
		&a_buffer.m_stagingBufAllocInfo));*/
}
#pragma endregion