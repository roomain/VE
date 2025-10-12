#pragma once
/***********************************************
* @headerfile VE_RenderingCtx.h
* @date 05 / 10 / 2025
* @author Roomain
************************************************/
#include "VE_FwdDecl.h"
#include "notCopiable.h"
#include "core_globals.h"

#pragma warning(push)
#pragma warning( disable : 4251 )
/*@brief base class for a rendering context,
* it contains all vulkan class for a rendering
* The context is used for one thread
*/
class VE_CORE_LIB VE_RenderingCtx
{
private:
	VE_GraphicalDevicePtr m_device;		/*!< rendering vulkan device*/
	VE_SwapChainPtr m_swapChain;		/*!< swapchain for rendering*/

	VkCommandBuffer m_cmdBuffer = VK_NULL_HANDLE; /*!< command buffer for the rendering context*/

public:
	NOT_COPIABLE(VE_RenderingCtx)
	VE_RenderingCtx() = delete;
	explicit VE_RenderingCtx(VE_GraphicalDevicePtr a_device);
	VE_RenderingCtx(VE_RenderingCtx&& a_other) noexcept = default;

	[[nodiscard]] inline VE_GraphicalDevicePtr device()const
	{
		return m_device;
	}
	[[nodiscard]] inline VkCommandBuffer commandBuffer() { return m_cmdBuffer; }
};

#pragma warning(pop)