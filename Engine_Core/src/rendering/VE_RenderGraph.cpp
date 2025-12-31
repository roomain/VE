#include "pch.h"
#include <ranges>
#include <functional>
#include "rendering/VE_RenderGraph.h"


VE_RenderGraph::VE_RenderGraph(const VE_DeviceContext& a_ctxt, const VkQueue a_renderQueue) : VulkanObject<VE_DeviceContext>(a_ctxt),
	m_renderQueue{ a_renderQueue }
{
	m_renderScene = std::make_shared<VE_RenderingScene>();
	// todo
	// create main buffer
	
}

void VE_RenderGraph::startProcess()
{
	static auto filter = [](const TaskPtr& a_task) {return !a_task->isStandBy(); };
	// reset active
	m_activeBuffer.clear();

	// Get active command buffers
	for (const auto& task : m_vTask | std::views::filter(filter))
		m_activeBuffer.emplace_back(task->commandBuffer());

	// create submit
	Vulkan::Initializers::SubmitInfoParameters parameters
	{
		.waitSemaphores = nullptr,
		.pipelineStages = {VK_PIPELINE_STAGE_ALL_GRAPHICS_BIT},
		.commandBuffers = m_activeBuffer.data(),
		.signalSemaphores = nullptr,
		.waitCount = 1,
		.bufferCount = static_cast<uint32_t>(m_activeBuffer.size()),
		.signalCount = 1
	};
	m_submitInfo = Vulkan::Initializers::submitInfo(parameters);
}

void VE_RenderGraph::finishProcess()
{
	if (m_mainCmdBuffer != VK_NULL_HANDLE && m_renderQueue != VK_NULL_HANDLE)
		VK_CHECK_LOG(vkQueueSubmit(m_renderQueue, 1, &m_submitInfo, VK_NULL_HANDLE))
}

bool VE_RenderGraph::registerComponent(const std::shared_ptr<VE_IComponent>& a_component)const
{
	auto pipeline = a_component->pipeline();
	// create pipeline if not exists
	if (!pipeline)
	{
		a_component->createPipeline(m_vkCtxt);
		pipeline = a_component->pipeline();
	}
	
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
