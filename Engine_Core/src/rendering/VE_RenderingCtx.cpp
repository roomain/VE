#include "pch.h"
#include "rendering/VE_RenderingCtx.h"
#include "rendering/components/VE_IComponent.h"
#include "VE_GraphicalDevice.h"
#include "VE_SwapChain.h"
#include "utils/VulkanInitializers.h"

VE_RenderingCtx::VE_RenderingCtx(VE_GraphicalDevicePtr a_device) : m_device{ a_device }
{

	// todo revoir command buffer dans VE_RenderGraph et tasks => unique_ptr
	m_swapChain = a_device->createNewSwapChain();
	// test
#pragma warning(push)
#pragma warning( disable : 4189 )// because unsed local variable
	m_swapChain->resize(256, 256);
	//unsigned int numImages = m_swapChain->swapBuffersCount();


	m_renderScene = std::make_shared<VE_RenderingScene>();
	if(auto queue = a_device->createQueue(VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_TRANSFER_BIT); queue != VK_NULL_HANDLE)
	{
		if(auto queueFamilyIt = a_device->findQueueFamily(VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_TRANSFER_BIT);
			queueFamilyIt != a_device->end())
		{
			// setup render graph
			m_renderGraph.setup(queue, queueFamilyIt->createCommandBuffer(VK_COMMAND_BUFFER_LEVEL_PRIMARY));
		}
	}

	//Buffer buffer = allocateStagingBuffer(m_device->context(),
	//	50 * sizeof(int));
#pragma warning(pop)

}

bool VE_RenderingCtx::registerComponent(const std::shared_ptr<VE_IComponent>& a_component)const
{
	auto pipeline = a_component->pipeline();
	if (!pipeline)
		pipeline = a_component->createPipeline(m_device->context());
	
	// register compoenent for rendering
	if (!a_component->isRegistered())
	{
		a_component->m_isRegistered = true;
		m_renderScene->componentsPerPipeline.add(pipeline, a_component);
		a_component->OnFlagChange.bind(std::bind_front(&VE_RenderingScene::onRenderFlagChanged, m_renderScene.get()));
		a_component->OnInvalidate.bind(std::bind_front(&VE_RenderingScene::onInvalidate, m_renderScene.get()));
		a_component->OnSelected.connect(std::bind_front(&VE_RenderingScene::onSelect, m_renderScene.get()));
		return true;
	}
	return false;
}

void VE_RenderingCtx::processRendering()
{
	if (m_presentSemaphore != VK_NULL_HANDLE)
	{
		VkSemaphoreCreateInfo semaphoreInfo = Vulkan::Initializers::semaphoreCreateInfo();
		VK_CHECK_EXCEPT(vkCreateSemaphore(m_device->context().m_logicalDevice, &semaphoreInfo, nullptr, &m_presentSemaphore))
	}

	uint32_t imageIndex = 0;
	VE_SwapChain::SwapChainBuffer image{};
	m_swapChain->acquireNextImage(m_presentSemaphore, nullptr, imageIndex, image);
	
	m_renderGraph.process();
	m_swapChain->present(m_renderGraph.renderQueue(), imageIndex, m_presentSemaphore);
}

void VE_RenderingCtx::onResize(const uint32_t a_width, const uint32_t a_height)
{
	m_swapChain->resize(a_width, a_height);
}