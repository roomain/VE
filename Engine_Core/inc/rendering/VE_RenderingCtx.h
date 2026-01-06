#pragma once
/***********************************************
* @headerfile VE_RenderingCtx.h
* @date 05 / 10 / 2025
* @author Roomain
************************************************/
#include "VE_FwdDecl.h"
#include "notCopiable.h"
#include "VE_RenderGraph.h"
#include "core_globals.h"

#pragma warning(push)
#pragma warning( disable : 4251 )
/*@brief base class for a rendering context,
* it contains all vulkan class for a rendering
*/
class VE_CORE_LIB VE_RenderingCtx
{
private:
	VE_GraphicalDevicePtr m_device;						/*!< rendering vulkan device*/
	VE_SwapChainPtr m_swapChain;						/*!< swapchain for rendering*/
	VkSemaphore m_presentSemaphore = VK_NULL_HANDLE;	/*!< semaphore to signal image presentation*/

	VE_RenderGraph m_renderGraph;		/*!< multithread rendering*/
	VE_RenderingScenePtr m_renderScene;	/*!< data shared between tasks*/

public:
	NOT_COPIABLE(VE_RenderingCtx)
	VE_RenderingCtx() = delete;
	explicit VE_RenderingCtx(VE_GraphicalDevicePtr a_device);
	VE_RenderingCtx(VE_RenderingCtx&& a_other) noexcept = default;
	[[nodiscard]] inline VE_GraphicalDevicePtr device()const
	{
		return m_device;
	}
	[[nodiscard]] bool registerComponent(const std::shared_ptr<VE_IComponent>& a_component)const;
	[[nodiscard]] VE_RenderingScenePtr renderingScene()const { return m_renderScene; }
	[[nodiscard]] VE_RenderGraph& renderGraph() { return m_renderGraph; }

	// todo setup render Graph width  precharged pipelines
	// void registerPipelines()
	void processRendering();

#pragma region callbacks
	void onResize(const uint32_t a_width, const uint32_t a_height);
#pragma endregion
};

#pragma warning(pop)