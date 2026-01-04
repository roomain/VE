#include "pch.h"
#include <ranges>
#include <functional>
#include "rendering/VE_RenderGraph.h"
#include "VE_GraphicalDevice.h"


void VE_RenderGraph::setup(VkQueue a_queue, VkCommandBuffer a_mainCmd)
{
	m_renderQueue = a_queue;
	m_mainCmdBuffer = a_mainCmd;
}

bool VE_RenderGraph::setEditTask(const VE_TaskParameters a_taskParameters)
{
	if (bHasEditTask)
		return true;

	if ((m_renderQueue != VK_NULL_HANDLE) && (m_mainCmdBuffer != VK_NULL_HANDLE))
	{
		insertTask<VE_RenderGraphEditTask>(a_taskParameters);
		bHasEditTask = true;
		return true;
	}
	return false;
}

bool VE_RenderGraph::addTasks(const std::vector<VE_TaskParametersEx>& a_tasksParameters)
{
	if ((m_renderQueue != VK_NULL_HANDLE) && (m_mainCmdBuffer != VK_NULL_HANDLE))
	{
		createTasks<VE_RenderGraphTask, VE_TaskParametersEx>(a_tasksParameters);
		return true;
	}
	return false;	
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
		.waitCount = 0,
		.bufferCount = static_cast<uint32_t>(m_activeBuffer.size()),
		.signalCount = 0
	};
	m_submitInfo = Vulkan::Initializers::submitInfo(parameters);
}

void VE_RenderGraph::finishProcess()
{
	if ((m_mainCmdBuffer != VK_NULL_HANDLE) && (m_renderQueue != VK_NULL_HANDLE))
		VK_CHECK_LOG(vkQueueSubmit(m_renderQueue, 1, &m_submitInfo, VK_NULL_HANDLE))
}
