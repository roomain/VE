#pragma once
#pragma once
/***********************************************
* @headerfile VulkanBufferInitializers.h
* @date 06 / 08 / 2025
* @author Roomain
************************************************/
#include "vulkan/vulkan.h"
#include "utils/VulkanDefines.h"

/*@brief functions for structure initialisation*/
namespace Vulkan::Initializers
{
	struct ImageViewParameters
	{
		VkFormat colorFormat = VK_FORMAT_UNDEFINED;
		VkImage image = VK_NULL_HANDLE;
		VkImageViewType viewType = VK_IMAGE_VIEW_TYPE_2D;
		VkImageViewCreateFlags flag = 0;
		VkImageSubresourceRange subResource = {
			.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
			.baseMipLevel = 0,
			.levelCount = 1,
			.baseArrayLayer = 0,
			.layerCount = 1
		};
	};

	[[nodiscard]] inline VkImageViewCreateInfo imageViewCreateInfo(
		const ImageViewParameters& a_parameters)
	{
		return VkImageViewCreateInfo{
			.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
			.pNext = nullptr,
			.flags = a_parameters.flag,
			.image = a_parameters.image,
			.viewType = a_parameters.viewType,
			.format = a_parameters.colorFormat,
			.components = {
				VK_COMPONENT_SWIZZLE_R,
				VK_COMPONENT_SWIZZLE_G,
				VK_COMPONENT_SWIZZLE_B,
				VK_COMPONENT_SWIZZLE_A
			},
			.subresourceRange = a_parameters.subResource
		};
	}


	struct BufferViewParameters
	{
		VkBufferViewCreateFlags flag = 0;
		VkBuffer bufferHandle = VK_NULL_HANDLE;
		VkFormat format = VK_FORMAT_UNDEFINED;
		VkDeviceSize offset = 0;
		VkDeviceSize range = VK_WHOLE_SIZE;
	};

	[[nodiscard]] inline VkBufferViewCreateInfo bufferViewCreateInfo(
		const BufferViewParameters& a_parameters)
	{
		return VkBufferViewCreateInfo{
			.sType = VK_STRUCTURE_TYPE_BUFFER_VIEW_CREATE_INFO,
			.pNext = nullptr,
			.flags = a_parameters.flag,
			.buffer = a_parameters.bufferHandle,
			.format = a_parameters.format,
			.offset = a_parameters.offset,
			.range = a_parameters.range
		};
	}


	[[nodiscard]] constexpr VkImageCreateInfo imageCreateInfo()
	{
		return VkImageCreateInfo{
			.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
			.pNext = nullptr,
			.imageType = VK_IMAGE_TYPE_2D
		};
	}

	struct BufferCreateInfoParameters
	{
		VkBufferCreateFlags flags = 0;
		VkDeviceSize size = 0;
		VkBufferUsageFlags usage = 0;
		VkSharingMode sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		uint32_t familyCount = 0;
		const uint32_t* familyIndex = nullptr;
	};

	template<typename IndexContainer>
	struct TBufferCreateInfoParameters
	{
		VkBufferCreateFlags flags = 0;
		VkDeviceSize size = 0;
		VkBufferUsageFlags usage = 0;
		VkSharingMode sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		const IndexContainer familyIndex = nullptr;
	};

	template<typename IndexContainer>
	[[nodiscard]] constexpr VkBufferCreateInfo bufferCreateInfo(
		const TBufferCreateInfoParameters<IndexContainer>& a_parameters)
	{
		return VkBufferCreateInfo{
			.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
			.pNext = nullptr,
			.flags = a_parameters.flags,
			.size = a_parameters.size,
			.usage = a_parameters.usage,
			.sharingMode = a_parameters.sharingMode,
			.queueFamilyIndexCount = static_cast<uint32_t>(a_parameters.familyIndex.size()),
			.pQueueFamilyIndices = a_parameters.familyIndex.data()
		};
	}

	[[nodiscard]] constexpr VkBufferCreateInfo bufferCreateInfo(const BufferCreateInfoParameters& params)
	{
		return VkBufferCreateInfo{
			.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
			.pNext = nullptr,
			.flags = params.flags,
			.size = params.size,
			.usage = params.usage,
			.sharingMode = params.sharingMode,
			.queueFamilyIndexCount = params.familyCount,
			.pQueueFamilyIndices = params.familyIndex
		};
	}

}