#include "pch.h"
#include "memory/VulkanImage.h"

VulkanBuffer allocateBuffer(const VE_DeviceContext& a_context, const VE_AllocationParameter& a_allocParameters, const std::vector<uint32_t>& a_sharedQueue)
{
	VkBufferCreateInfo stagingBufInfo = Vulkan::Initializers::bufferCreateInfo(
		Vulkan::Initializers::BufferCreateInfoParameters{
		.flags = 0,
		.size = a_allocParameters.m_size,
		.usage = a_allocParameters.m_usage,
		.sharingMode = VK_SHARING_MODE_CONCURRENT,
		.familyCount = static_cast<uint32_t>(a_sharedQueue.size()),
		.familyIndex = a_sharedQueue.data()
		});

	VmaAllocationCreateInfo bufAllocCreateInfo = {};
	bufAllocCreateInfo.usage = VMA_MEMORY_USAGE_AUTO_PREFER_DEVICE;
	bufAllocCreateInfo.flags = a_allocParameters.m_allocFlag;

	VulkanBuffer buffer;

	VK_CHECK_EXCEPT(vmaCreateBuffer(a_context.m_memAllocator,
		&stagingBufInfo,
		&bufAllocCreateInfo,
		&buffer.m_buffer,
		&buffer.m_Alloc,
		&buffer.m_AllocInfo))

		return buffer;
}

VulkanBuffer allocateBuffer(const VE_DeviceContext& a_context, const VE_AllocationParameter& a_allocParameters)
{
	VkBufferCreateInfo stagingBufInfo = Vulkan::Initializers::bufferCreateInfo(Vulkan::Initializers::BufferCreateInfoParameters{
		.flags = 0,
		.size = a_allocParameters.m_size,
		.usage = a_allocParameters.m_usage,
		.sharingMode = VK_SHARING_MODE_EXCLUSIVE,
		.familyCount = 0,
		.familyIndex = nullptr
		});

	const VmaAllocationCreateInfo bufAllocCreateInfo{
		.flags = a_allocParameters.m_allocFlag,
		.usage = VMA_MEMORY_USAGE_AUTO_PREFER_DEVICE
	};

	VulkanBuffer buffer;

	VK_CHECK_EXCEPT(vmaCreateBuffer(a_context.m_memAllocator,
		&stagingBufInfo,
		&bufAllocCreateInfo,
		&buffer.m_buffer,
		&buffer.m_Alloc,
		&buffer.m_AllocInfo))

		return buffer;
}

VulkanBuffer allocateStagingBuffer(const VE_DeviceContext& a_context, const size_t& a_bufferByteSize)
{
	VkBufferCreateInfo stagingBufInfo = Vulkan::Initializers::bufferCreateInfo(Vulkan::Initializers::BufferCreateInfoParameters{
		.flags = 0,
		.size = a_bufferByteSize,
		.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		.sharingMode = VK_SHARING_MODE_EXCLUSIVE,
		.familyCount = 0,
		.familyIndex = nullptr
		});

	const VmaAllocationCreateInfo stagingBufAllocCreateInfo {
		.flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT | VMA_ALLOCATION_CREATE_MAPPED_BIT,
		.usage = VMA_MEMORY_USAGE_AUTO
	};

	VulkanBuffer stagingBuffer;

	VK_CHECK_EXCEPT(vmaCreateBuffer(a_context.m_memAllocator,
		&stagingBufInfo,
		&stagingBufAllocCreateInfo,
		&stagingBuffer.m_buffer,
		&stagingBuffer.m_Alloc,
		&stagingBuffer.m_AllocInfo))

		return stagingBuffer;
}

void releaseBuffer(const VE_DeviceContext& a_context, VulkanBuffer& a_buffer)
{
	vmaDestroyBuffer(a_context.m_memAllocator, a_buffer.m_buffer, a_buffer.m_Alloc);
	a_buffer.m_buffer = VK_NULL_HANDLE;
	a_buffer.m_Alloc = VK_NULL_HANDLE;
}

VulkanImage allocateStagingImage(const VE_DeviceContext& a_context, const VkImageCreateInfo& a_imageCreateInfo)
{
	VulkanImage image;

	const VmaAllocationCreateInfo alloCreateInfo{
		.flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT | VMA_ALLOCATION_CREATE_MAPPED_BIT,
		.usage = VMA_MEMORY_USAGE_AUTO
	};

	VK_CHECK_EXCEPT(vmaCreateImage(a_context.m_memAllocator, &a_imageCreateInfo, &alloCreateInfo, &image.m_image, &image.m_Alloc, &image.m_AllocInfo))
	return image;
}

VulkanImage allocateImage(const VE_DeviceContext& a_context, const VkImageCreateInfo& a_imageCreateInfo)
{
	VulkanImage image;

	const VmaAllocationCreateInfo alloCreateInfo{
		.flags = //VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT | VMA_ALLOCATION_CREATE_MAPPED_BIT,
		.usage = //VMA_MEMORY_USAGE_AUTO
	};

	VK_CHECK_EXCEPT(vmaCreateImage(a_context.m_memAllocator, &a_imageCreateInfo, &alloCreateInfo, &image.m_image, &image.m_Alloc, &image.m_AllocInfo))
	return image;
}

void releaseImage(const VE_DeviceContext& a_context, VulkanImage& a_image)
{
	vmaDestroyImage(a_context.m_memAllocator, a_image.m_image, a_image.m_Alloc);
	a_image.m_image = VK_NULL_HANDLE;
	a_image.m_Alloc = VK_NULL_HANDLE;
}