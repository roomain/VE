#include "pch.h"

#include "rendering/VE_RenderGraphTask.h"
#include "rendering/components/VE_Component.h"

void VE_RenderGraphTask::process(const VE_GraphData& a_data)
{
    if (m_cmdBuffer != VK_NULL_HANDLE)
    {
        vkBeginCommandBuffer(m_cmdBuffer, /*todo*/);
        for (auto pipelineId : m_pipelineIds)
        {
            auto pipeline = a_data.m_registeredPipelines.at(pipelineId);
            //pipeline->
            for (auto weakCmp : a_data.m_componentByPipelineId.at(pipelineId))
            {
                if (auto component = weakCmp.lock())
                {
                    component->writeCommands(a_data.m_renderCtx, m_cmdBuffer);
                }
            }
            //pipeline->
        }
        vkEndCommandBuffer(m_cmdBuffer);
    }
}