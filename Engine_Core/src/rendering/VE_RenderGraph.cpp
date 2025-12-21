#include "pch.h"
#include "rendering/VE_RenderGraph.h"


VE_RenderGraph::VE_RenderGraph(const VE_DeviceContext& a_ctxt, const VkQueue a_renderQueue) : VulkanObject<VE_DeviceContext>(a_ctxt),
	m_renderQueue{ a_renderQueue }
{
	m_renderScene = std::make_shared< VE_RenderingScene>(a_ctxt);
	// todo
	// create main buffer
	// create submit
	Vulkan::Initializers::SubmitInfoParameters parameters
	{
		.waitSemaphores = nullptr,
		.pipelineStages = {VK_PIPELINE_STAGE_ALL_GRAPHICS_BIT},
		.commandBuffers = nullptr,
		.signalSemaphores = nullptr,
		.waitCount = 1,
		.bufferCount = 1,
		.signalCount = 1
	};
	m_submitInfo = Vulkan::Initializers::submitInfo(parameters);
}

void VE_RenderGraph::startProcess()
{
	// todo
}

void VE_RenderGraph::finishProcess()
{
	if (m_mainCmdBuffer != VK_NULL_HANDLE && m_renderQueue != VK_NULL_HANDLE)
	{
		//VK_CHECK_LOG(vkQueueSubmit(m_renderQueue, 1, &m_submitInfo, VK_NULL_HANDLE));
	}
}

bool VE_RenderGraph::registerComponent(const std::shared_ptr<VE_IComponent>& a_component)
{
	if (a_component->hasFlag<RenderingFlagBit::IS_RENDERING>())
	{
		auto pipeline = a_component->pipeline();
		if (!pipeline)
		{
			a_component->createPipeline(m_vkCtxt);
			pipeline = a_component->pipeline();
			// register
		}


		if (a_component->hasFlag<RenderingFlagBit::IS_EDITABLE>())
		{
			// in edit rendering command
		}
		else
		{
			// in static rendering command
		}
	}
	else
	{
		// remove from rendering commands
	}
	
	return false;
}
