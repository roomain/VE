#pragma once
/***********************************************
* @headerfile VE_GraphicalDevice.h
* @date 19 / 07 / 2025
* @author Roomain
************************************************/
#include <memory>
#include "VE_Device.h"
#include "core_globals.h"

class VE_SwapChain;
class VE_InstanceCapabilities;

#pragma warning(push)
#pragma warning(disable:4251) // disable warning for dll export/import
class VE_CORE_LIB VE_GraphicalDevice : public VE_Device
{
	friend class VE_Application;
	friend class VE_RenderingCtx;
private:
	std::shared_ptr<VE_SwapChain> m_swapChain;	/*!< rendering swapchain*/
	int32_t m_presentationQueueIndex = -1;		/*!< presentation queue index*/
	VkSurfaceKHR m_surface = VK_NULL_HANDLE;

	explicit VE_GraphicalDevice(const VE_InstanceCapabilities& a_capabilities, const VE_DeviceContext& a_context, const std::vector<int>& a_queueFamilies,
		VkSurfaceKHR a_surface, const int a_presentQueueIndex);

	[[nodiscard]] std::shared_ptr<VE_SwapChain> createNewSwapChain();
public:
	virtual ~VE_GraphicalDevice()override;
	[[nodiscard]] std::shared_ptr<VE_SwapChain> swapChain()const;
};
#pragma warning(pop)