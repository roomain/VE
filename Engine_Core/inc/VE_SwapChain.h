#pragma once
/***********************************************
* @headerfile VE_SwapChain.h
* @date 19 / 07 / 2025
* @author Roomain
************************************************/
#include <vector>
#include "notCopiable.h"
#include "vulkan/vulkan.hpp"
#include "utils/VulkanContext.h"
#include "core_globals.h"

#pragma warning(push)
#pragma warning(disable:4251) // disable warning for dll export/import
class VE_CORE_LIB VE_SwapChain : public VulkanObject<VE_SwapChainContext>
{
	friend class VE_GraphicalDevice;
private:

	struct SwapChainBuffer
	{
		VkImage m_image = VK_NULL_HANDLE;			/*!< image buffer*/
		VkImageView m_imageView = VK_NULL_HANDLE;	/*!< image view*/
	};
	
	std::vector<SwapChainBuffer> m_swapChainsBuffers;	/*!< image buffer to display*/

	void createSwapChain(const uint32_t a_width, const uint32_t a_height);
	void releaseSwapchain(VkSwapchainKHR a_oldSwapChain);
	void createImageBuffers(const VkFormat a_colorFormat);

	/*@brief get available resolution approaching desired*/
	[[nodiscard]] static VkExtent2D getImageExtent(const VkSurfaceCapabilitiesKHR& a_surfCaps, const uint32_t a_width, const uint32_t a_height);
	/*@brief get image count for multi buffering*/
	[[nodiscard]] static uint32_t getImageCount(const VkSurfaceCapabilitiesKHR& a_surfCaps);

	/*@brief find best data parameter with the current context*/
	[[nodiscard]] static VkSurfaceFormatKHR findSurfaceFormat(const VE_SwapChainContext& a_ctxt);
	[[nodiscard]] static VkCompositeAlphaFlagBitsKHR findCompositeAlpha(const VkSurfaceCapabilitiesKHR& a_surfCaps);


	void acquireNextImage(VkSemaphore a_presentCompleteSemaphore, VkFence a_fence, uint32_t& a_imageIndex, SwapChainBuffer& a_image)const;
	void present(VkQueue a_presentationQueue, const uint32_t a_imageIndex, VkSemaphore a_waitSemaphore)const;
	void present(VkQueue a_presentationQueue, const uint32_t a_imageIndex, std::vector<VkSemaphore>& a_waitSemaphore)const;
	
	VE_SwapChain(const VE_DeviceContext& a_ctxt, VkSurfaceKHR a_surface);

public:
	NOT_COPIABLE(VE_SwapChain)
	virtual ~VE_SwapChain()override;
	void resize(const uint32_t a_width, const uint32_t a_height);
	[[nodiscard]] uint32_t swapBuffersCount()const noexcept;
};
#pragma warning(pop)