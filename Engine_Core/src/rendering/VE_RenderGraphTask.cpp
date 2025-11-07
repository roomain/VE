#include "pch.h"
#include "rendering/pipelines/VE_GraphicalPipeline.h"
#include "rendering/VE_RenderGraphTask.h"
#include "rendering/components/VE_IComponent.h"
#include "utils/VulkanCmdInitializers.h"


void VE_RenderGraphTask::process(const VE_GraphData& a_data)
{
    if (m_cmdBuffer == VK_NULL_HANDLE)
        return;
    
    VkCommandBufferBeginInfo cmbBeginInfo = Vulkan::Initializers::commandBufferBeginInfo(m_usageFlag, nullptr);

    vkBeginCommandBuffer(m_cmdBuffer, &cmbBeginInfo);
    for (auto pipelineId : m_pipelineIds)
    {
        auto pipeline = a_data.m_registeredPipelines.at(pipelineId);
        pipeline->bind(m_cmdBuffer);
        for (auto weakCmp : a_data.m_componentByPipelineId.at(pipelineId))
        {
            if (auto component = weakCmp.lock())
            {
                if (component->m_bRenderEnable)
                    component->writeCommands(a_data.m_renderCtx, m_cmdBuffer);
            }
        }
    }
    vkEndCommandBuffer(m_cmdBuffer);
}

void taskCallback(const std::shared_ptr<VE_RenderGraphTask>& a_task, const VE_GraphData& a_data)
{
    a_task->process(a_data);
}