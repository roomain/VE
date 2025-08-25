#pragma once
/***********************************************
* @headerfile VulkanImageInitializers.h
* @date 06 / 08 / 2025
* @author Roomain
************************************************/
#include "vulkan/vulkan.h"
#include "utils/VulkanDefines.h"

/*@brief functions for structure initialisation*/
namespace Vulkan::Initializers
{

	
	struct ImageCreateInfoParameters
	{
		uint32_t width = 1024;
		uint16_t height = 1024;
		/*
		VK_IMAGE_USAGE_TRANSFER_SRC_BIT
			Image can be used as the source of a copy/blit (e.g. saving rendered image to a buffer).

		VK_IMAGE_USAGE_TRANSFER_DST_BIT
			Image can be used as the destination of a copy/blit (e.g. uploading texture data).

		VK_IMAGE_USAGE_SAMPLED_BIT
			Image can be sampled in shaders via a sampler (texture() in GLSL/HLSL).

		VK_IMAGE_USAGE_STORAGE_BIT
			Image can be bound as a storage image in shaders, allowing read/write (imageLoad, imageStore).

		VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT
			Image can be used as a color render target in a framebuffer.

		VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT
			Image can be used as a depth/stencil render target.

		VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT
			Image is a temporary attachment; content is not preserved after rendering (drivers may use lazily allocated memory).
			Useful for depth buffers in deferred rendering.

		VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT
			Image can be used as an input attachment in a subpass (directly read by fragment shaders without a sampler).
		*/
		VkImageUsageFlags usage = VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
		/*
		Color formats
			VK_FORMAT_R8G8B8A8_UNORM : 8 bits per channel, unsigned normalized, RGBA.
			VK_FORMAT_B8G8R8A8_SRGB : 8-bit BGRA with sRGB gamma correction.
			Used for: textures, framebuffers.

		Depth/stencil formats
			VK_FORMAT_D32_SFLOAT : 32-bit floating-point depth.
			VK_FORMAT_D24_UNORM_S8_UINT : 24-bit depth + 8-bit stencil.
			Used for: depth/stencil attachments.

		Integer formats
			VK_FORMAT_R16_UINT : single-channel, 16-bit unsigned integer.
			VK_FORMAT_R32G32B32_SINT : signed 32-bit integers in RGB.
			Used for: compute, storage images, vertex data.

		Compressed formats
			VK_FORMAT_BC1_RGB_UNORM_BLOCK : block-compressed DXT1-style.
			VK_FORMAT_ASTC_4x4_UNORM_BLOCK : ASTC compression.
			Used for: GPU-efficient textures.
		*/
		VkFormat format = VK_FORMAT_R8G8B8A8_SRGB;
	};

	[[nodiscard]] constexpr VkImageCreateInfo image2DCreateInfo(const ImageCreateInfoParameters& a_parameters)
	{
		return VkImageCreateInfo{
			.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
			.pNext = nullptr,
			.flags = 0,
			.imageType = VK_IMAGE_TYPE_2D,
			.format = a_parameters.format,
			.extent = { a_parameters.width, a_parameters.height, 1 },
			.mipLevels = 1,
			.arrayLayers = 1,
			.samples = VK_SAMPLE_COUNT_1_BIT,
			.tiling = VK_IMAGE_TILING_OPTIMAL,
			.usage = a_parameters.usage,
			.sharingMode = VK_SHARING_MODE_EXCLUSIVE,
			.queueFamilyIndexCount = 0,
			.pQueueFamilyIndices = nullptr,
			.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED
		};	
	}

	[[nodiscard]] constexpr VkImageCreateInfo image2DCreateInfo(const ImageCreateInfoParameters& a_parameters, const std::vector<uint32_t>& a_sharedQueues)
	{
		return VkImageCreateInfo{
			.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
			.pNext = nullptr,
			.flags = 0,
			.imageType = VK_IMAGE_TYPE_2D,
			.format = a_parameters.format,
			.extent = { a_parameters.width, a_parameters.height, 1 },
			.mipLevels = 1,
			.arrayLayers = 1,
			.samples = VK_SAMPLE_COUNT_1_BIT,
			.tiling = VK_IMAGE_TILING_OPTIMAL,
			.usage = a_parameters.usage,
			.sharingMode = VK_SHARING_MODE_CONCURRENT,
			.queueFamilyIndexCount = static_cast<uint32_t>(a_sharedQueues.size()),
			.pQueueFamilyIndices = a_sharedQueues.data(),
			.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED
		};
	}
}
