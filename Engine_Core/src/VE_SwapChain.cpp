#include "pch.h"
#include "VE_SwapChain.h"
#include "enumerate.h"
#include "utils/VulkanInitializers.h"
#include "utils/VulkanBufferInitializers.h"

VkExtent2D VE_SwapChain::getImageExtent(const VkSurfaceCapabilitiesKHR& a_surfCaps, const uint32_t a_width, const uint32_t a_height)
{
	VkExtent2D extent = a_surfCaps.currentExtent;
	if (a_surfCaps.currentExtent.width == (uint32_t)-1)
	{
		// If the surface size is undefined, the size is set to
		// the size of the images requested.
		extent.width = a_width;
		extent.height = a_height;
	}
	return extent;
}

uint32_t VE_SwapChain::getImageCount(const VkSurfaceCapabilitiesKHR& a_surfCaps)
{
	uint32_t desiredNumberOfSwapchainImages = a_surfCaps.minImageCount + 1;
	if ((a_surfCaps.maxImageCount > 0) && (desiredNumberOfSwapchainImages > a_surfCaps.maxImageCount))
	{
		desiredNumberOfSwapchainImages = a_surfCaps.maxImageCount;
	}
	return desiredNumberOfSwapchainImages;
}

VkSurfaceFormatKHR VE_SwapChain::findSurfaceFormat(const VE_SwapChainContext& a_ctxt)
{
	std::vector<VkSurfaceFormatKHR> surfaceFormats;
	enumerate(&vkGetPhysicalDeviceSurfaceFormatsKHR, surfaceFormats, a_ctxt.m_physicalDevice, a_ctxt.m_surface);

	VkSurfaceFormatKHR surfaceFormat = surfaceFormats[0];

	// format un preferential order
	const std::vector<VkFormat> preferredImageFormats = {
		VK_FORMAT_R8G8B8A8_UNORM,
		VK_FORMAT_B8G8R8A8_UNORM,
		VK_FORMAT_A8B8G8R8_UNORM_PACK32
	};

	// find compatible format
	for (const auto& availableFormat : surfaceFormats) {
		if (std::ranges::find(preferredImageFormats, availableFormat.format) != preferredImageFormats.end()) {
			surfaceFormat = availableFormat;
			break;
		}
	}

	return surfaceFormat;
}

VkCompositeAlphaFlagBitsKHR VE_SwapChain::findCompositeAlpha(const VkSurfaceCapabilitiesKHR& a_surfCaps)
{
	VkCompositeAlphaFlagBitsKHR compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	static constexpr std::array<VkCompositeAlphaFlagBitsKHR, 4>  compositeAlphaFlags = {
		VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
		VK_COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR,
		VK_COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR,
		VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR,
	};

	for (const auto& compositeAlphaFlag : compositeAlphaFlags) {
		if (a_surfCaps.supportedCompositeAlpha & compositeAlphaFlag) {
			compositeAlpha = compositeAlphaFlag;
			break;
		}
	}

	return compositeAlpha;
}

void VE_SwapChain::releaseSwapchain(VkSwapchainKHR a_oldSwapChain)
{
	for (const auto& img : m_swapChainsBuffers)
		vkDestroyImageView(m_vkCtxt.m_logicalDevice, img.m_imageView, nullptr);
	vkDestroySwapchainKHR(m_vkCtxt.m_logicalDevice, a_oldSwapChain, nullptr);
	m_swapChainsBuffers.clear();
}

void VE_SwapChain::createImageBuffers(const VkFormat a_colorFormat)
{
	std::vector<VkImage> images;
	VK_CHECK_EXCEPT(enumerateEx(&vkGetSwapchainImagesKHR, images, VK_SUCCESS, m_vkCtxt.m_logicalDevice, m_vkCtxt.m_swapChain))
	
	for (const VkImage img : images)
	{
		VkImageView imageView = VK_NULL_HANDLE;

		const Vulkan::Initializers::ImageViewParameters parameters
		{
			.colorFormat = a_colorFormat,
			.image = img,
			.viewType = VK_IMAGE_VIEW_TYPE_2D,
			.flag = 0,
			.subResource = VkImageSubresourceRange{
				.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
				.baseMipLevel = 0,
				.levelCount = 1,
				.baseArrayLayer = 0,
				.layerCount = 1
			}
		};

		VkImageViewCreateInfo colorAttachmentView = Vulkan::Initializers::imageViewCreateInfo(parameters);


		VK_CHECK_EXCEPT(vkCreateImageView(m_vkCtxt.m_logicalDevice, &colorAttachmentView, nullptr, &imageView))

		m_swapChainsBuffers.emplace_back(SwapChainBuffer{
			img,
			imageView
			});
	}
}

void VE_SwapChain::createSwapChain(const uint32_t a_width, const uint32_t a_height)
{
	// Get physical device surface properties and formats
	VkSurfaceCapabilitiesKHR surfCaps;
	VK_CHECK_EXCEPT(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_vkCtxt.m_physicalDevice, m_vkCtxt.m_surface, &surfCaps))

	std::vector<VkPresentModeKHR> presentationModes;
	enumerate(&vkGetPhysicalDeviceSurfacePresentModesKHR, presentationModes, m_vkCtxt.m_physicalDevice, m_vkCtxt.m_surface);

	const VkExtent2D swapchainExtent = VE_SwapChain::getImageExtent(surfCaps, a_width, a_height);

	// Determine the number of images in swapchain
	const uint32_t desiredNumberOfSwapchainImages = VE_SwapChain::getImageCount(surfCaps);


	// The VK_PRESENT_MODE_FIFO_KHR mode must always be present as per spec
	// This mode waits for the vertical blank ("v-sync")
	VkPresentModeKHR swapchainPresentMode = VK_PRESENT_MODE_FIFO_KHR;
	VkSurfaceFormatKHR imageFormat = VE_SwapChain::findSurfaceFormat(m_vkCtxt);

	VkImageUsageFlags usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | surfCaps.supportedUsageFlags;

	// create info with the current swapchain
	VkSwapchainCreateInfoKHR swapchainCI = Vulkan::Initializers::swapChainCreateInfoKHR(m_vkCtxt.m_surface, imageFormat,
		desiredNumberOfSwapchainImages, swapchainExtent, usage, surfCaps.currentTransform, 
		VE_SwapChain::findCompositeAlpha(surfCaps), swapchainPresentMode, VK_TRUE, m_vkCtxt.m_swapChain);


	VK_CHECK_EXCEPT(vkCreateSwapchainKHR(m_vkCtxt.m_logicalDevice, &swapchainCI, nullptr, &m_vkCtxt.m_swapChain))

	if (swapchainCI.oldSwapchain != VK_NULL_HANDLE)
	{
		releaseSwapchain(swapchainCI.oldSwapchain);
	}

	createImageBuffers(imageFormat.format);
}

VE_SwapChain::VE_SwapChain(const VE_DeviceContext& a_ctxt, VkSurfaceKHR a_surface) : 
	VulkanObject<VE_SwapChainContext>(a_ctxt)
{
	m_vkCtxt.m_surface = a_surface;
}

VE_SwapChain::~VE_SwapChain()
{
	releaseSwapchain(m_vkCtxt.m_swapChain);
}

void VE_SwapChain::resize(const uint32_t a_width, const uint32_t a_height)
{
	if (a_width == 0 || a_height == 0)
	{
		releaseSwapchain(m_vkCtxt.m_swapChain);
	}
	else
	{
		createSwapChain(a_width, a_height);
	}
}

uint32_t VE_SwapChain::swapBuffersCount()const noexcept
{
	return static_cast<uint32_t>(m_swapChainsBuffers.size());
}

void VE_SwapChain::acquireNextImage(VkSemaphore a_presentCompleteSemaphore, VkFence a_fence, uint32_t& a_imageIndex, SwapChainBuffer& a_image)const
{
	VK_CHECK_LOG(vkAcquireNextImageKHR(m_vkCtxt.m_logicalDevice, m_vkCtxt.m_swapChain, UINT64_MAX, a_presentCompleteSemaphore, a_fence, &a_imageIndex))
	a_image = m_swapChainsBuffers[a_imageIndex];
}

void VE_SwapChain::present(VkQueue a_presentationQueue, const uint32_t a_imageIndex, VkSemaphore a_waitSemaphore)const
{
	auto presentCI = Vulkan::Initializers::presentationKHR(1, &a_waitSemaphore, 1, &m_vkCtxt.m_swapChain, &a_imageIndex);
	VK_CHECK_LOG(vkQueuePresentKHR(a_presentationQueue, &presentCI))
}

void VE_SwapChain::present(VkQueue a_presentationQueue, const uint32_t a_imageIndex, std::vector<VkSemaphore>& a_waitSemaphore)const
{
	auto presentCI = Vulkan::Initializers::presentationKHR(static_cast<uint32_t>(a_waitSemaphore.size()),
		a_waitSemaphore.data(), 1, &m_vkCtxt.m_swapChain, &a_imageIndex);
	VK_CHECK_LOG(vkQueuePresentKHR(a_presentationQueue, &presentCI))
}